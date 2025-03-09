{
	description = "OpenGL tests";

	inputs = {
		nixpkgs.url = "nixpkgs/nixos-24.11";
	};

	outputs = { self, nixpkgs }: let
		version = builtins.substring 0 8 self.lastModifiedDate;
		supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];
		forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
		allpkgs = forAllSystems (system: import nixpkgs { inherit system; });

		runPkgs = pkgs: with pkgs; [ libGL libGL.dev libglvnd libglvnd.dev mesa.drivers glfw ];
		buildPkgs = pkgs: with pkgs; [ gcc gnumake ];
	in
	{
		# nix build .#<name>
		packages = forAllSystems (system:
			let pkgs = allpkgs.${system}; in
			{
				opengl-test = pkgs.stdenv.mkDerivation {
					pname = "opengl-test";
					inherit version;
					src = ./.;

					nativeBuildInputs = buildPkgs pkgs;
					buildInputs = runPkgs pkgs;

					buildPhase = ''
						make bin/opengl-test
					'';

					installPhase = ''
						mkdir -p $out/bin
						mv bin/opengl-test $out/bin
					'';
				};
			}
		);

		# nix run .#<name>
		apps = forAllSystems(system: {
			opengl-test = {
				program = "${self.packages.${system}.opengl-test}/bin/opengl-test";
				type = "app";
			};
		});

		# nix develop
		devShell = forAllSystems(
			system:
			let pkgs = allpkgs.${system}; in
				pkgs.mkShell {

					nativeBuildInputs = buildPkgs pkgs ++ (
						with pkgs; [
							xorg.libX11.dev
							xorg.libXinerama
							xorg.libXft
							xorg.libXcursor
							xorg.libXrandr
							xorg.libXi.dev
						]
					);

					buildInputs = runPkgs pkgs;

					shellHook = ''
						LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib";
						PROMPT=$'%F{8} %~ %B%F{6}$%f%b ' zsh && exit
					'';
				}
			);
	};
}
