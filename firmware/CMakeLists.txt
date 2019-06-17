#
# Copyright (c) 2017, 2018, The MCUSim Contributors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the MCUSim or its parts nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Xling firmware build configuration.
#
cmake_minimum_required(VERSION 3.2)
project(Xling C)

# Set common variables
set(TARGET_OUTPUT_BASENAME "xling")
set(TARGET_OUTPUT_FILE "${TARGET_OUTPUT_BASENAME}.elf")
set(TARGET_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(AVR_MCU "atmega328p")
set(AVR_FREQ 12000000UL)

# Remove '-rdynamic', '-Wl,-search_paths_first' (avr-gcc doesn't support this
# one and treats it as '-Wl,-s' which strips linked ELF
set(CMAKE_C_LINK_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

# Set flags
if (CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Building DEBUG version of Xling firmware")
	add_definitions("-g -DDEBUG -mmcu=${AVR_MCU} -DF_CPU=${AVR_FREQ}")

	add_definitions("-std=iso9899:1999")
	add_definitions("-Wall")
	add_definitions("-pedantic")
	add_definitions("-Wshadow")
	add_definitions("-Wpointer-arith")
	add_definitions("-Wcast-qual")
	add_definitions("-Wcast-align")
	add_definitions("-Wstrict-prototypes")
	add_definitions("-Wmissing-prototypes")
	add_definitions("-Wconversion")
	add_definitions("-Wsign-compare")
	add_definitions("-Wmissing-declarations")
	add_definitions("-Wnested-externs")
	add_definitions("-Wbad-function-cast")
	add_definitions("-Wold-style-definition")
	add_definitions("-Wunused")
	add_definitions("-Wuninitialized")
	add_definitions("-Wmissing-noreturn")
	add_definitions("-Wmissing-format-attribute")
	add_definitions("-Wredundant-decls")
	#add_definitions("-Werror")
	add_definitions("-Werror=implicit")
	add_definitions("-Werror=nonnull")
	add_definitions("-Werror=init-self")
	add_definitions("-Werror=main")
	add_definitions("-Werror=missing-braces")
	add_definitions("-Werror=sequence-point")
	add_definitions("-Werror=return-type")
	add_definitions("-Werror=trigraphs")
	add_definitions("-Werror=array-bounds")
	add_definitions("-Werror=write-strings")
	add_definitions("-Werror=address")
	add_definitions("-Werror=int-to-pointer-cast")
	add_definitions("-Werror=pointer-to-int-cast")
else()
	message(STATUS "Building RELEASE version of Xling firmware")
	add_definitions("-Os -mmcu=${AVR_MCU} -DF_CPU=${AVR_FREQ}")

	add_definitions("-std=iso9899:1999")
	add_definitions("-Wall")
	add_definitions("-pedantic")
	add_definitions("-Wshadow")
	add_definitions("-Wpointer-arith")
	add_definitions("-Wcast-qual")
	add_definitions("-Wcast-align")
	add_definitions("-Wstrict-prototypes")
	add_definitions("-Wmissing-prototypes")
	add_definitions("-Wconversion")
	add_definitions("-Wsign-compare")
	add_definitions("-Wmissing-declarations")
	add_definitions("-Wnested-externs")
	add_definitions("-Wbad-function-cast")
	add_definitions("-Wold-style-definition")
	add_definitions("-Wunused")
	add_definitions("-Wuninitialized")
	add_definitions("-Wmissing-noreturn")
	add_definitions("-Wmissing-format-attribute")
	add_definitions("-Wredundant-decls")
	#add_definitions("-Werror")
	add_definitions("-Werror=implicit")
	add_definitions("-Werror=nonnull")
	add_definitions("-Werror=init-self")
	add_definitions("-Werror=main")
	add_definitions("-Werror=missing-braces")
	add_definitions("-Werror=sequence-point")
	add_definitions("-Werror=return-type")
	add_definitions("-Werror=trigraphs")
	add_definitions("-Werror=array-bounds")
	add_definitions("-Werror=write-strings")
	add_definitions("-Werror=address")
	add_definitions("-Werror=int-to-pointer-cast")
	add_definitions("-Werror=pointer-to-int-cast")
endif()

# Set linker flags
if (CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmcu=${AVR_MCU}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.map,--cref,--section-start=.text=0")
	message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")
else()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmcu=${AVR_MCU}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.map,--cref,--section-start=.text=0")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")
endif()

# Find executables
find_program(AVR_CC avr-gcc)
#find_program(AVR_CXX avr-g++)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_OBJDUMP avr-objdump)
find_program(AVR_DUDE avrdude)
find_program(SREC_CAT srec_cat)

# Define mandatory variables
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})

# Define includes
include_directories("../")
include_directories("include/")

# Set sources here
set(XLING_SRC
	src/fuse.c
	src/main.c
	src/twi/twibb.c
	src/oled/sh1106.c
	src/graphics/sh1106.c
)

add_executable(${TARGET_OUTPUT_FILE} ${XLING_SRC})
add_custom_target("upload")

# Prepare files for MCU
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_SIZE_TOOL} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_FILE})
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJDUMP} -h -S ${TARGET_OUTPUT_FILE} > ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.lss)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJCOPY} -R .eeprom -R .fuse -R .lock -R .signature -O ihex ${TARGET_OUTPUT_FILE} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.hex)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJCOPY} --no-change-warnings -j .fuse --change-section-lma .fuse=0 -O ihex ${TARGET_OUTPUT_FILE} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.fuse)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJCOPY} --no-change-warnings -j .eeprom --change-section-lma .eeprom=0 -O ihex ${TARGET_OUTPUT_FILE} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.eep)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJCOPY} --no-change-warnings -j .lock --change-section-lma .lock=0 -O ihex ${TARGET_OUTPUT_FILE} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.lock)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJCOPY} --no-change-warnings -j .signature --change-section-lma .signature=0 -O ihex ${TARGET_OUTPUT_FILE} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.sig)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${SREC_CAT} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.fuse -Intel -crop 0x00 0x01 -offset  0x00 -O ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.lfs -Intel)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${SREC_CAT} ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.fuse -Intel -crop 0x01 0x02 -offset -0x01 -O ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.hfs -Intel)
add_custom_command(
	TARGET ${TARGET_OUTPUT_FILE} POST_BUILD
	COMMAND ${AVR_OBJDUMP} -m avr -D ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.hex > ${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.hex.txt)

add_custom_command(
	TARGET "upload" POST_BUILD
	COMMAND ${AVR_DUDE} -p m328p -b 115200 -P /dev/ttyU0 -c stk500v2 -Uflash:w:${TARGET_OUTPUT_DIR}/${TARGET_OUTPUT_BASENAME}.hex)
