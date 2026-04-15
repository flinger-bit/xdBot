# xdBot Remastered

Port and cleanup pass for the current Geode / Geometry Dash toolchain.

## Targets
- Geode 5.6.1
- Geometry Dash 2.2081
- Windows + Android64

## Build
- Local: set `GEODE_SDK` and run `geode build`
- GitHub: push to the repo and let `.github/workflows/build.yml` produce the packaged output

## Notes
- `mod.json` is pinned to the exact Geode version expected by this package
- Android and Windows are the only platforms wired in for this build
