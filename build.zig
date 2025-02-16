const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const my_test = b.addExecutable(.{
        .name = "MySTL_test",
        .target = target,
        .optimize = optimize,
    });
    my_test.linkLibCpp();
    my_test.addIncludePath(b.path("include"));
    my_test.addCSourceFiles(.{
        .root = b.path("test"),
        .files = &.{
            "main.cpp",
            "test_array.cpp",
        },
        .flags = &.{
            "-std=c++23",
            "-Wall",
            "-Wextra",
        },
    });

    b.installArtifact(my_test);

    const run_test = b.addRunArtifact(my_test);
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_test.step);
}
