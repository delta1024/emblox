const std = @import("std");

const program_version = std.SemanticVersion{
    .major = 0,
    .minor = 0,
    .patch = 0,
    .pre = "alpha",
};
// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const conf = b.addConfigHeader(.{ .include_path = "lox_common.h" }, .{
        .VERSION = b.fmt("{}", .{program_version}),
    });

    const lib = b.addSharedLibrary(.{
        .name = "loxcore",
        // In this case the main source file is merely a path, however, in more
        // complicated build scripts, this could be a generated file.
        .target = target,
        .optimize = optimize,
        .link_libc = true,
        .version = program_version,
    });

    lib.addCSourceFiles(.{ .files = try getSrcFiles(b) });
    lib.addIncludePath(.{ .path = "include" });
    lib.addIncludePath(.{ .path = "src" });
    lib.installHeadersDirectory(.{ .path = "include" }, "", .{});
    lib.addConfigHeader(conf);

    b.installArtifact(lib);
    try addEditorConfStep(b, lib, conf);
    setupLibraryTests(lib, target, optimize);
}

fn addEditorConfStep(b: *std.Build, lib: *std.Build.Step.Compile, conf: *std.Build.Step.ConfigHeader) !void {
    var data = std.ArrayList(u8).init(b.allocator);
    for (lib.root_module.include_dirs.items) |inc| switch (inc) {
        std.Build.Module.IncludeDir.path, .path_system => |p| try data.appendSlice(b.fmt("-I{s}\n", .{p.getPath(b)})),
        else => {},
    };
    try data.appendSlice("-I");

    const tee = b.addSystemCommand(&.{ "dd", "status=none", "of=compile_flags.txt" });
    tee.setStdIn(.{ .bytes = data.items });
    const editor_conf = b.step("editor_conf", "get generated config header");

    const echo = b.addSystemCommand(&.{"dirname"});
    echo.addFileArg(conf.getOutput());
    echo.step.dependOn(&conf.step);
    const app = b.addSystemCommand(&.{ "dd", "status=none", "oflag=append", "conv=notrunc", "of=compile_flags.txt" });
    app.setStdIn(.{ .lazy_path = echo.captureStdOut() });
    app.step.dependOn(&echo.step);
    app.step.dependOn(&tee.step);
    editor_conf.dependOn(&app.step);
}

fn getSrcFiles(b: *std.Build) ![]const []const u8 {
    const src_dir = try b.build_root.handle.openDir("src", .{ .iterate = true });
    var c_files = std.ArrayList([]const u8).init(b.allocator);
    var src_walker = try src_dir.walk(b.allocator);
    while (try src_walker.next()) |entry| {
        if (entry.kind == .file and std.mem.endsWith(u8, entry.path, ".c")) {
            try c_files.append(b.pathJoin(&.{ "src", entry.path }));
        }
    }
    return c_files.items;
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
