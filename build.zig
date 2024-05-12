const std = @import("std");

const program_version = std.SemanticVersion{
    .major = 0,
    .minor = 0,
    .patch = 0,
    .pre = "alpha",
};
const cur_tag = "main";

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const conf = b.addConfigHeader(.{ .include_path = "lox_config.h" }, .{
        .LOX_VERSION = b.fmt("lox {}", .{program_version}),
        .LOX_VERSION_MAJOR = @as(i64, @intCast(program_version.major)),
        .LOX_VERSION_MINOR = @as(i64, @intCast(program_version.minor)),
        .LOX_VERSION_PATCH = @as(i64, @intCast(program_version.patch)),
    });

    const lib = b.addSharedLibrary(.{
        .name = "lox",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .version = program_version,
    });

    lib.addCSourceFiles(.{ .root = .{ .path = "src/lib" }, .files = try getSrcFiles(b, "src/lib") });
    lib.addIncludePath(.{ .path = "include" });
    lib.addIncludePath(.{ .path = "src" });
    lib.installHeadersDirectory(.{ .path = "include" }, "", .{});
    lib.addConfigHeader(conf);

    b.installArtifact(lib);

    try setupEditorConfStep(b, lib, conf);
    setupLibraryTests(lib, target, optimize);
    setupCleanSteps(b);
    setupDistStep(b);
    setupUninstallStep(b);

    const bin = b.addExecutable(.{
        .name = "lox",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .version = program_version,
    });
    bin.addCSourceFiles(.{ .root = .{ .path = "src/bin/" }, .files = try getSrcFiles(b, "src/bin") });
    bin.linkLibrary(lib);
    b.installArtifact(bin);

    const run_step = b.step("run", "run the binary");

    const run_cmd = b.addRunArtifact(bin);
    if (b.args) |args|
        run_cmd.addArgs(args);

    run_step.dependOn(&run_cmd.step);

    const dryrun_step = b.step("dryrun", "build targets but do not install them");
    dryrun_step.dependOn(&lib.step);
    dryrun_step.dependOn(&bin.step);
}

fn setupUninstallStep(b: *std.Build) void {
    const files_to_remove = [_][]const u8{
        b.getInstallPath(.{ .lib = {} }, b.fmt(
            "liblox.so.{d}.{d}.{d}",
            .{ program_version.major, program_version.minor, program_version.patch },
        )),
        b.getInstallPath(.{ .lib = {} }, b.fmt("liblox.so.{d}", .{program_version.major})),
        b.getInstallPath(.{ .lib = {} }, "liblox.so"),
        b.getInstallPath(.{ .header = {} }, "lox.h"),
    };
    const rm_step = b.getUninstallStep();
    for (files_to_remove) |path| {
        const step = b.addRemoveDirTree(path);
        rm_step.dependOn(&step.step);
    }
}
fn setupDistStep(b: *std.Build) void {
    const dist_step = b.step("dist", "package program for destrabution");
    const dist_cmd = b.addSystemCommand(&.{
        "git",
        "archive",
        "--format=tar.gz",
        "-o",
        b.fmt("lox-{}.tar.gz", .{program_version}),
        "--prefix=lox/",
        cur_tag,
    });
    dist_step.dependOn(&dist_cmd.step);
}
fn setupCleanSteps(b: *std.Build) void {
    const clean_targets = [_]*std.Build.Step.RemoveDir{
        b.addRemoveDirTree("zig-out"),
        b.addRemoveDirTree("zig-cache"),
        b.addRemoveDirTree("compile_flags.txt"),
    };
    const dist_clean = b.step("distclean", "clean build directory for distrabution tarball");
    for (clean_targets) |step|
        dist_clean.dependOn(&step.step);
    const clean = b.step("clean", "remove all files generated by the build step");
    for (clean_targets[0..2]) |targ|
        clean.dependOn(&targ.step);
}

fn setupLibraryTests(lib: *std.Build.Step.Compile, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode) void {
    const b = lib.step.owner;
    const lib_unit_tests = b.addTest(.{
        .root_source_file = b.path("src/test.zig"),
        .target = target,
        .optimize = optimize,
    });
    lib_unit_tests.installLibraryHeaders(lib);
    lib_unit_tests.linkLibrary(lib);
    lib_unit_tests.addRPath(lib.getEmittedBinDirectory());

    const run_lib_unit_tests = b.addRunArtifact(lib_unit_tests);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_lib_unit_tests.step);
}
fn setupEditorConfStep(b: *std.Build, lib: *std.Build.Step.Compile, conf: *std.Build.Step.ConfigHeader) !void {
    var data = std.ArrayList(u8).init(b.allocator);
    for (lib.root_module.include_dirs.items) |inc| switch (inc) {
        std.Build.Module.IncludeDir.path, .path_system => |p| try data.appendSlice(b.fmt("-I{s}\n", .{p.getPath(b)})),
        else => {},
    };
    try data.appendSlice("-xc\n-xc-header\n-I");

    const tee = b.addSystemCommand(&.{ "dd", "status=none", "of=compile_flags.txt" });
    tee.setStdIn(.{ .bytes = data.items });

    const editor_conf = b.step("editorconf", "generate compile_flags.txt");

    const echo = b.addSystemCommand(&.{"dirname"});
    echo.addFileArg(conf.getOutput());
    echo.step.dependOn(&conf.step);
    const app = b.addSystemCommand(&.{ "dd", "status=none", "oflag=append", "conv=notrunc", "of=compile_flags.txt" });
    app.setStdIn(.{ .lazy_path = echo.captureStdOut() });
    app.step.dependOn(&echo.step);
    app.step.dependOn(&tee.step);
    editor_conf.dependOn(&app.step);
}
fn getSrcFiles(b: *std.Build, prefix: []const u8) ![]const []const u8 {
    const src_dir = try b.build_root.handle.openDir(prefix, .{ .iterate = true });
    var c_files = std.ArrayList([]const u8).init(b.allocator);
    var src_walker = try src_dir.walk(b.allocator);
    while (try src_walker.next()) |entry| {
        if (entry.kind == .file and std.mem.endsWith(u8, entry.path, ".c")) {
            try c_files.append(entry.path);
        }
    }
    return c_files.items;
}
