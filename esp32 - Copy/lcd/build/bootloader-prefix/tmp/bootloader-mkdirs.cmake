# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/mnt/d/esp32/esp-idf/components/bootloader/subproject"
  "/mnt/d/esp32/lcd/build/bootloader"
  "/mnt/d/esp32/lcd/build/bootloader-prefix"
  "/mnt/d/esp32/lcd/build/bootloader-prefix/tmp"
  "/mnt/d/esp32/lcd/build/bootloader-prefix/src/bootloader-stamp"
  "/mnt/d/esp32/lcd/build/bootloader-prefix/src"
  "/mnt/d/esp32/lcd/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/d/esp32/lcd/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
