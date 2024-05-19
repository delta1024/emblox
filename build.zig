const std = @import("std");
const Build = std.Build;
const program_name = "lox";
const include_dir = "include";
const lib_dir = "src/lib";
const bin_dir = "src/bin";
const LibLinkMode = enum {
    Static,
    Dynamic,
};

const CompileFlags = struct {
    step: Build.Step,
    include: Build.LazyPath,
    lib: Build.LazyPath,
};
pub fn build(b: *Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const lib_link = b.option(LibLinkMode, "link_lib", "Set the library output type.") orelse .Dynamic;

    const config = b.addConfigHeader(.{ .include_path = "lox_config.h" }, .{
        .UINT8_SIZE = std.math.maxInt(u8),
    });
    const lib = switch (lib_link) {
        .Static => b.addStaticLibrary(.{
            .name = program_name,
            .target = target,
            .optimize = optimize,
            .pic = true,
        }),
        .Dynamic => b.addSharedLibrary(.{
            .name = program_name,
            .target = target,
            .optimize = optimize,
            .pic = true,
        }),
    };
    lib.addConfigHeader(config);
    lib.linkLibC();
    lib.addIncludePath(b.path(include_dir));
    lib.installHeadersDirectory(b.path(include_dir), "", .{});
    var lib_src_dir = try b.build_root.handle.openDir(lib_dir, .{
        .iterate = true,
    });
    defer lib_src_dir.close();
    var src_files = std.ArrayList([]const u8).init(b.allocator);
    var walker = try lib_src_dir.walk(b.allocator);
    while (try walker.next()) |file| {
        if (std.mem.eql(u8, std.fs.path.extension(file.basename), ".c")) {
            try src_files.append(b.dupe(file.basename));
        }
    }
    lib.addCSourceFiles(.{
        .root = b.path(lib_dir),
        .files = src_files.items,
    });

    b.installArtifact(lib);

    const exe = b.addExecutable(.{
        .name = "lox",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    exe.addCSourceFile(.{
        .file = b.path(b.pathJoin(&.{ bin_dir, "main.c" })),
    });
    exe.linkLibrary(lib);
    exe.addRPath(lib.getEmittedBinDirectory());
    exe.addIncludePath(b.path(include_dir));

    b.installArtifact(exe);

    const run_step = b.step("run", "run the executable");
    const run_cmd = b.addRunArtifact(exe);
    if (b.args) |args|
        run_cmd.addArgs(args);
    run_step.dependOn(&run_cmd.step);

    const general_compile_flags =
        b.fmt(
        \\-xc
        \\-xc-header
        \\-I{s}
        \\
    , .{try b.build_root.handle.realpathAlloc(b.allocator, include_dir)});
    const lib_compile_flags = b.fmt(
        \\{s}-I{s}
        \\-I
    , .{
        general_compile_flags,
        try b.build_root.handle.realpathAlloc(b.allocator, lib_dir),
    });

    const write_flags = b.addWriteFiles();
    write_flags.addBytesToSource(lib_compile_flags, "compile_flags.txt");

    const dirname = b.addSystemCommand(&.{"dirname"});
    dirname.addFileArg(config.getOutput());

    const dd = b.addSystemCommand(&.{ "dd", "status=none", "oflag=append", "conv=notrunc", "of=compile_flags.txt" });
    dd.setStdIn(.{ .lazy_path = dirname.captureStdOut() });
    dd.step.dependOn(&write_flags.step);
    const editorconf_step = b.step("editorconf", "generate compile_flags.txt");
    editorconf_step.dependOn(&dd.step);
}
