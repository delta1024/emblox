const std = @import("std");

const program_version = std.SemanticVersion{
    .major = 0,
    .minor = 0,
    .patch = 0,
    .pre = "alpha",
};
const cur_tag = "main";

const BuildOpts = struct {
    trace_execution: bool = true,
    fn parse(b: *std.Build) BuildOpts {
        var opts: BuildOpts = .{};
        if (b.option(bool, "trace_stack_execution", "print stack info and current instruction")) |t|
            opts.trace_execution = t;
        return opts;
    }
};

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const opts = BuildOpts.parse(b);
    const conf = b.addConfigHeader(.{ .include_path = "lox_config.h" }, .{
        .LOX_VERSION = b.fmt("lox {}", .{program_version}),
        .LOX_VERSION_MAJOR = @as(i64, @intCast(program_version.major)),
        .LOX_VERSION_MINOR = @as(i64, @intCast(program_version.minor)),
        .LOX_VERSION_PATCH = @as(i64, @intCast(program_version.patch)),
        .UINT8_MAX = @as(i64, @intCast(std.math.maxInt(u8) + 1)),
    });

    const lib = b.addSharedLibrary(.{
        .name = "lox",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .version = program_version,
    });
    if (opts.trace_execution)
        lib.defineCMacro("DEBUG_TRACE_EXECUTION", null);
    lib.addCSourceFiles(.{ .root = .{ .path = "src/lib/" }, .files = &.{
        "lox.c",
        "chunk.c",
        "memory.c",
        "vm.c",
        "debug.c",
        "value.c",
        "lox/state.c",
        "lox/stack.c",
        "lox/value.c",
    } });
    lib.addIncludePath(.{ .path = "include" });
    lib.addIncludePath(b.path("src/lib"));
    lib.installHeadersDirectory(.{ .path = "include" }, "", .{});
    lib.addConfigHeader(conf);

    const lib_install = b.addInstallArtifact(lib, .{});
    b.getInstallStep().dependOn(&lib_install.step);
    try setupEditorConfStep(b, lib, conf, opts);
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
    bin.addCSourceFiles(.{ .root = .{ .path = "src/bin/" }, .files = &.{"main.c"} });
    bin.installLibraryHeaders(lib);
    bin.addIncludePath(b.path("include"));
    bin.addLibraryPath(.{ .path = b.getInstallPath(.{ .lib = {} }, "") });
    bin.addRPath(.{ .path = b.getInstallPath(.{ .lib = {} }, "") });
    bin.linkSystemLibrary("lox");
    bin.step.dependOn(&lib_install.step);
    b.installArtifact(bin);

    const run_step = b.step("run", "run the binary");

    const run_cmd = b.addRunArtifact(bin);
    if (b.args) |args|
        run_cmd.addArgs(args);

    run_step.dependOn(&run_cmd.step);

    const dryrun_step = b.step("dryrun", "build targets but do not install them");
    dryrun_step.dependOn(&lib.step);
    dryrun_step.dependOn(&bin.step);

    const doc_step = b.step("doc", "generate documentation");
    const docopen_step = b.step("docopen", "open generated documentation");
    const doc_run = b.addSystemCommand(&.{
        "doxygen",
        "Doxyfile",
    });
    doc_step.dependOn(&doc_run.step);

    const doc_open_cmd = b.addSystemCommand(&.{
        "firefox",
        "./doc/index.html",
    });
    doc_open_cmd.step.dependOn(&doc_run.step);
    docopen_step.dependOn(&doc_open_cmd.step);

    const docclean_step = b.step("docclean", "clean up generated documentation directory");
    const docclean_cmd = b.addRemoveDirTree("doc");
    docclean_step.dependOn(&docclean_cmd.step);
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
        b.addRemoveDirTree("src/lib/compile_flags.txt"),
        b.addRemoveDirTree("src/bin/compile_flags.txt"),
    };
    const clean = b.step("clean", "remove all files generated by the build step");
    for (&clean_targets) |targ|
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
fn setupEditorConfStep(b: *std.Build, lib: *std.Build.Step.Compile, conf: *std.Build.Step.ConfigHeader, opts: BuildOpts) !void {
    var data = std.ArrayList(u8).init(b.allocator);
    for (lib.root_module.include_dirs.items) |inc| switch (inc) {
        std.Build.Module.IncludeDir.path, .path_system => |p| try data.appendSlice(b.fmt("-I{s}\n", .{p.getPath(b)})),
        else => {},
    };
    if (opts.trace_execution)
        try data.appendSlice("-DDEBUG_TRACE_EXECUTION\n");

    try data.appendSlice("-xc\n-xc-header\n-I");

    const tee = b.addSystemCommand(&.{ "dd", "status=none", "of=src/lib/compile_flags.txt" });
    tee.setStdIn(.{ .bytes = data.items });

    const editor_conf = b.step("editorconf", "generate compile_flags.txt");

    const echo = b.addSystemCommand(&.{"dirname"});
    echo.addFileArg(conf.getOutput());
    echo.step.dependOn(&conf.step);
    const app = b.addSystemCommand(&.{ "dd", "status=none", "oflag=append", "conv=notrunc", "of=src/lib/compile_flags.txt" });
    app.setStdIn(.{ .lazy_path = echo.captureStdOut() });
    app.step.dependOn(&echo.step);
    app.step.dependOn(&tee.step);
    editor_conf.dependOn(&app.step);

    const bin_file = b.addSystemCommand(&.{ "dd", "status=none", "of=src/bin/compile_flags.txt" });
    bin_file.setStdIn(.{ .bytes = "-Iinclude\n-xc\n-xc-header" });
    editor_conf.dependOn(&bin_file.step);
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
