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

pub fn build(b: *Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const lib_link = b.option(LibLinkMode, "link_lib", "Set the library output type.") orelse .Dynamic;

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
}
