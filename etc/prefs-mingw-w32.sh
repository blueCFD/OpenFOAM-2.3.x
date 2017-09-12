#----------------------------------*-sh-*--------------------------------------
# License
#     Copyright (C) 2011-2014 blueCAPE Lda
#
#     This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
#     For more information about these patches, visit:
#         http://bluecfd.com/Core
#
#     This file is a derivative work of OpenFOAM.
#
#     OpenFOAM is free software: you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
#     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#     for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
#
# File
#     etc/prefs-mingw-w32.sh
#
# Description
#     Preset variables for the OpenFOAM configuration - POSIX shell syntax.
#
#     This file will only be sourced if set when calling bashrc. Example:
#       source $HOME/OpenFOAM/OpenFOAM-2.0/etc/bashrc prefs-mingw-w32.sh
#
#------------------------------------------------------------------------------

#- Operating System:
#    WM_OSTYPE = POSIX | MSwindows
export WM_OSTYPE=MSwindows

# Specify compiler mode: system or ThirdParty
# ~~~~~~~~~~~~~~~~~~~~~~~
foamCompiler=ThirdParty

# Specify openmpi
# ~~~~~~~~~~~~~~~~~~~~~~
export WM_MPLIB=OPENMPI

#- Cross-compilers: WM_COMPILER = 
#       mingw32    - custom build of the Gcc+mingw cross-compiler
#       mingw-w32  - custom build of the 32bit Gcc+mingw-w64 cross-compiler
#       mingw-w64  - custom build of the 64bit Gcc+mingw-w64 cross-compiler
export WM_COMPILER=mingw-w32
unset WM_COMPILER_ARCH WM_COMPILER_LIB_ARCH

#- Architecture:
#    WM_ARCH_OPTION = 32 | 64
export WM_ARCH_OPTION=32

# ----------------------------------------------------------------- end-of-file
