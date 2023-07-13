# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/gabriel/esp/esp-idf/components/bootloader/subproject"
  "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader"
  "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix"
  "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix/tmp"
  "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix/src/bootloader-stamp"
  "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix/src"
  "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/gabriel/github-classroom/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/fse-trabalho3-adnecaiogabriel/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
