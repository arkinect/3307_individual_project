Package: fmt:x64-linux@12.1.0

**Host Environment**

- Host: x64-linux
- Compiler: GNU 13.3.0
- CMake Version: 3.31.10
-    vcpkg-tool version: 2025-12-16-44bb3ce006467fc13ba37ca099f64077b8bbf84d
    vcpkg-scripts version: 6d7bf7ef21 2026-01-30 (2 days ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached fmtlib-fmt-12.1.0.tar.gz
-- Cleaning sources at /home/mathew/vcpkg/buildtrees/fmt/src/12.1.0-54f1f91231.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source /home/mathew/vcpkg/downloads/fmtlib-fmt-12.1.0.tar.gz
-- Using source at /home/mathew/vcpkg/buildtrees/fmt/src/12.1.0-54f1f91231.clean
-- Configuring x64-linux
-- Building x64-linux-dbg
-- Building x64-linux-rel
-- Fixing pkgconfig file: /home/mathew/vcpkg/packages/fmt_x64-linux/lib/pkgconfig/fmt.pc
CMake Error at scripts/cmake/vcpkg_find_acquire_program.cmake:201 (message):
  Could not find pkg-config.  Please install it via your package manager:

      sudo apt-get install pkg-config
Call Stack (most recent call first):
  scripts/cmake/vcpkg_fixup_pkgconfig.cmake:193 (vcpkg_find_acquire_program)
  ports/fmt/portfile.cmake:19 (vcpkg_fixup_pkgconfig)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "keystone-xl-analysis",
  "version-string": "0.1.0",
  "dependencies": [
    "fmt",
    "nlohmann-json"
  ]
}

```
</details>
