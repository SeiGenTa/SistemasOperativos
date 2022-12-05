cmd_/media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o :=  gcc-10 -Wp,-MMD,/media/sf_SistemasOperativos/Modules2020-2/Syncread/.kmutex.o.d -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/10/include -I/usr/src/linux-headers-5.10.0-19-common/arch/x86/include -I./arch/x86/include/generated -I/usr/src/linux-headers-5.10.0-19-common/include -I./include -I/usr/src/linux-headers-5.10.0-19-common/arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I/usr/src/linux-headers-5.10.0-19-common/include/uapi -I./include/generated/uapi -include /usr/src/linux-headers-5.10.0-19-common/include/linux/kconfig.h -include /usr/src/linux-headers-5.10.0-19-common/include/linux/compiler_types.h -D__KERNEL__ -fmacro-prefix-map=/usr/src/linux-headers-5.10.0-19-common/= -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -std=gnu89 -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fcf-protection=none -m64 -falign-jumps=1 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mpreferred-stack-boundary=3 -mskip-rax-setup -mtune=generic -mno-red-zone -mcmodel=kernel -DCONFIG_X86_X32_ABI -Wno-sign-compare -fno-asynchronous-unwind-tables -mindirect-branch=thunk-extern -mindirect-branch-register -mfunction-return=thunk-extern -fno-jump-tables -fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 -fno-allow-store-data-races -Wframe-larger-than=2048 -fstack-protector-strong -Wimplicit-fallthrough -Wno-unused-but-set-variable -Wno-unused-const-variable -g -pg -mrecord-mcount -mfentry -DCC_USING_FENTRY -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wno-stringop-truncation -Wno-zero-length-bounds -Wno-array-bounds -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wno-packed-not-aligned -Wall -std=gnu99  -DMODULE  -DKBUILD_BASENAME='"kmutex"' -DKBUILD_MODNAME='"syncread"' -c -o /media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o /media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.c

source_/media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o := /media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.c

deps_/media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o := \
  /usr/src/linux-headers-5.10.0-19-common/include/linux/kconfig.h \
    $(wildcard include/config/cc/version/text.h) \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  /usr/src/linux-headers-5.10.0-19-common/include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/cc/has/asm/inline.h) \
  /usr/src/linux-headers-5.10.0-19-common/include/linux/compiler_attributes.h \
  /usr/src/linux-headers-5.10.0-19-common/include/linux/compiler-gcc.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/retpoline.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
    $(wildcard include/config/kcov.h) \

/media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o: $(deps_/media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o)

$(deps_/media/sf_SistemasOperativos/Modules2020-2/Syncread/kmutex.o):
