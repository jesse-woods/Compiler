{
  description = "A pure C development environment using Clang";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        
        # Override the standard environment to use Clang instead of GCC
        # This fixes standard library header paths for Clang and Clangd automatically.
        clangEnv = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; };
      in
      {
        devShells.default = clangEnv {
          packages = with pkgs; [
            # Build Tooling
            cmake
            gnumake
            
            # Diagnostic & LSP Tools
            clang-tools # Provides wrapped clangd, clang-format, and clang-tidy
            valgrind    # Memory leak detection (Linux only)
            gdb         # Debugger
          ];

          shellHook = ''
            echo "========================================="
            echo "⚡ Pure Clang C Development Environment ⚡"
            echo "Compiler: $(clang --version | head -n 1)"
            echo "========================================="
          '';
        };
      });
}
