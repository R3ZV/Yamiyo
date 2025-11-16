const std = @import("std");

const Compile = std.Build.Step.Compile;
const OptimizeMode = std.builtin.OptimizeMode;
const ResolvedTarget = std.Build.ResolvedTarget;

const CPP_FLAGS = .{
    "-std=c++20",
    "-O2",
    "-pedantic",
    "-Wall",
    "-Wextra",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wformat=2",
    "-Wnull-dereference",
    "-fstack-protector-strong",
    "-D_FORTIFY_SOURCE=2",
};

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    build_bin(b, target, optimize);
}

const bin_files = .{ "app/main.cpp" } ++ src_files ++ lib_files;
const src_files = .{
    "src/entity.cpp",
    "src/game.cpp",
    "src/spatial-hashing.cpp",
};

const lib_files = .{
    "libs/imgui/imgui.cpp",
    "libs/imgui/imgui_impl_sdl3.cpp",
    "libs/imgui/imgui_impl_sdlrenderer3.cpp", 
    "libs/imgui/imgui_draw.cpp", 
    "libs/imgui/imgui_demo.cpp", 
    "libs/imgui/imgui_widgets.cpp", 
    "libs/imgui/imgui_tables.cpp", 
};

fn build_bin(
    b: *std.Build,
    target: ResolvedTarget,
    optimize: OptimizeMode,
) void {
    const sdl3_image = b.dependency("SDL_image", .{}).artifact("SDL3_image");

    const exe = b.addExecutable(.{
        .name = "yamiyo",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .imports = &.{},
        }),
    });

    exe.linkLibrary(sdl3_image);
    exe.linkLibCpp();
    exe.linkSystemLibrary("SDL3");

    exe.addCSourceFiles(.{
        .files = &bin_files,
        .flags = &CPP_FLAGS,
    });

    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the binary");
    run_step.dependOn(&run_cmd.step);

    b.installArtifact(exe);

}
