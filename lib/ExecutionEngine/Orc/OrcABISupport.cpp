//===------------- OrcABISupport.cpp - ABI specific support code ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/ExecutionEngine/Orc/OrcABISupport.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Process.h"

namespace llvm {
namespace orc {

void OrcAArch64::writeResolverCode(uint8_t *ResolverMem, JITReentryFn ReentryFn,
                                   void *CallbackMgr) {

  const uint32_t ResolverCode[] = {
    // resolver_entry:
    0xa9bf47fd,        // 0x000:  stp  x29, x17, [sp, #-16]!
    0x910003fd,        // 0x004:  mov  x29, sp
    0xa9bf73fb,        // 0x008:  stp  x27, x28, [sp, #-16]!
    0xa9bf6bf9,        // 0x00c:  stp  x25, x26, [sp, #-16]!
    0xa9bf63f7,        // 0x010:  stp  x23, x24, [sp, #-16]!
    0xa9bf5bf5,        // 0x014:  stp  x21, x22, [sp, #-16]!
    0xa9bf53f3,        // 0x018:  stp  x19, x20, [sp, #-16]!
    0xa9bf3fee,        // 0x01c:  stp  x14, x15, [sp, #-16]!
    0xa9bf37ec,        // 0x020:  stp  x12, x13, [sp, #-16]!
    0xa9bf2fea,        // 0x024:  stp  x10, x11, [sp, #-16]!
    0xa9bf27e8,        // 0x028:  stp   x8,  x9, [sp, #-16]!
    0xa9bf1fe6,        // 0x02c:  stp   x6,  x7, [sp, #-16]!
    0xa9bf17e4,        // 0x030:  stp   x4,  x5, [sp, #-16]!
    0xa9bf0fe2,        // 0x034:  stp   x2,  x3, [sp, #-16]!
    0xa9bf07e0,        // 0x038:  stp   x0,  x1, [sp, #-16]!
    0xadbf7ffe,        // 0x03c:  stp  q30, q31, [sp, #-32]!
    0xadbf77fc,        // 0x040:  stp  q28, q29, [sp, #-32]!
    0xadbf6ffa,        // 0x044:  stp  q26, q27, [sp, #-32]!
    0xadbf67f8,        // 0x048:  stp  q24, q25, [sp, #-32]!
    0xadbf5ff6,        // 0x04c:  stp  q22, q23, [sp, #-32]!
    0xadbf57f4,        // 0x050:  stp  q20, q21, [sp, #-32]!
    0xadbf4ff2,        // 0x054:  stp  q18, q19, [sp, #-32]!
    0xadbf47f0,        // 0x058:  stp  q16, q17, [sp, #-32]!
    0xadbf3fee,        // 0x05c:  stp  q14, q15, [sp, #-32]!
    0xadbf37ec,        // 0x060:  stp  q12, q13, [sp, #-32]!
    0xadbf2fea,        // 0x064:  stp  q10, q11, [sp, #-32]!
    0xadbf27e8,        // 0x068:  stp   q8,  q9, [sp, #-32]!
    0xadbf1fe6,        // 0x06c:  stp   q6,  q7, [sp, #-32]!
    0xadbf17e4,        // 0x070:  stp   q4,  q5, [sp, #-32]!
    0xadbf0fe2,        // 0x074:  stp   q2,  q3, [sp, #-32]!
    0xadbf07e0,        // 0x078:  stp   q0,  q1, [sp, #-32]!
    0x580004e0,        // 0x07c:  ldr   x0, Lcallbackmgr
    0xaa1e03e1,        // 0x080:  mov   x1, x30
    0xd1003021,        // 0x084:  sub   x1,  x1, #12
    0x58000442,        // 0x088:  ldr   x2, Lreentry_fn_ptr
    0xd63f0040,        // 0x08c:  blr   x2
    0xaa0003f1,        // 0x090:  mov   x17, x0
    0xacc107e0,        // 0x094:  ldp   q0,  q1, [sp], #32
    0xacc10fe2,        // 0x098:  ldp   q2,  q3, [sp], #32
    0xacc117e4,        // 0x09c:  ldp   q4,  q5, [sp], #32
    0xacc11fe6,        // 0x0a0:  ldp   q6,  q7, [sp], #32
    0xacc127e8,        // 0x0a4:  ldp   q8,  q9, [sp], #32
    0xacc12fea,        // 0x0a8:  ldp  q10, q11, [sp], #32
    0xacc137ec,        // 0x0ac:  ldp  q12, q13, [sp], #32
    0xacc13fee,        // 0x0b0:  ldp  q14, q15, [sp], #32
    0xacc147f0,        // 0x0b4:  ldp  q16, q17, [sp], #32
    0xacc14ff2,        // 0x0b8:  ldp  q18, q19, [sp], #32
    0xacc157f4,        // 0x0bc:  ldp  q20, q21, [sp], #32
    0xacc15ff6,        // 0x0c0:  ldp  q22, q23, [sp], #32
    0xacc167f8,        // 0x0c4:  ldp  q24, q25, [sp], #32
    0xacc16ffa,        // 0x0c8:  ldp  q26, q27, [sp], #32
    0xacc177fc,        // 0x0cc:  ldp  q28, q29, [sp], #32
    0xacc17ffe,        // 0x0d0:  ldp  q30, q31, [sp], #32
    0xa8c107e0,        // 0x0d4:  ldp   x0,  x1, [sp], #16
    0xa8c10fe2,        // 0x0d8:  ldp   x2,  x3, [sp], #16
    0xa8c117e4,        // 0x0dc:  ldp   x4,  x5, [sp], #16
    0xa8c11fe6,        // 0x0e0:  ldp   x6,  x7, [sp], #16
    0xa8c127e8,        // 0x0e4:  ldp   x8,  x9, [sp], #16
    0xa8c12fea,        // 0x0e8:  ldp  x10, x11, [sp], #16
    0xa8c137ec,        // 0x0ec:  ldp  x12, x13, [sp], #16
    0xa8c13fee,        // 0x0f0:  ldp  x14, x15, [sp], #16
    0xa8c153f3,        // 0x0f4:  ldp  x19, x20, [sp], #16
    0xa8c15bf5,        // 0x0f8:  ldp  x21, x22, [sp], #16
    0xa8c163f7,        // 0x0fc:  ldp  x23, x24, [sp], #16
    0xa8c16bf9,        // 0x100:  ldp  x25, x26, [sp], #16
    0xa8c173fb,        // 0x104:  ldp  x27, x28, [sp], #16
    0xa8c17bfd,        // 0x108:  ldp  x29, x30, [sp], #16
    0xd65f0220,        // 0x10c:  ret  x17
    0x01234567,        // 0x110:  Lreentry_fn_ptr:
    0xdeadbeef,        // 0x114:      .quad 0
    0x98765432,        // 0x118:  Lcallbackmgr:
    0xcafef00d         // 0x11c:      .quad 0
  };

  const unsigned ReentryFnAddrOffset = 0x110;
  const unsigned CallbackMgrAddrOffset = 0x118;

  memcpy(ResolverMem, ResolverCode, sizeof(ResolverCode));
  memcpy(ResolverMem + ReentryFnAddrOffset, &ReentryFn, sizeof(ReentryFn));
  memcpy(ResolverMem + CallbackMgrAddrOffset, &CallbackMgr,
         sizeof(CallbackMgr));
}

void OrcAArch64::writeTrampolines(uint8_t *TrampolineMem, void *ResolverAddr,
                                  unsigned NumTrampolines) {

  unsigned OffsetToPtr = alignTo(NumTrampolines * TrampolineSize, 8);

  memcpy(TrampolineMem + OffsetToPtr, &ResolverAddr, sizeof(void *));

  // OffsetToPtr is actually the offset from the PC for the 2nd instruction, so
  // subtract 32-bits.
  OffsetToPtr -= 4;

  uint32_t *Trampolines = reinterpret_cast<uint32_t *>(TrampolineMem);

  for (unsigned I = 0; I < NumTrampolines; ++I, OffsetToPtr -= TrampolineSize) {
    Trampolines[3 * I + 0] = 0xaa1e03f1;                      // mov x17, x30
    Trampolines[3 * I + 1] = 0x58000010 | (OffsetToPtr << 3); // adr x16, Lptr
    Trampolines[3 * I + 2] = 0xd63f0200;                      // blr x16
  }

}

Error OrcAArch64::emitIndirectStubsBlock(IndirectStubsInfo &StubsInfo,
                                         unsigned MinStubs,
                                         void *InitialPtrVal) {
  // Stub format is:
  //
  // .section __orc_stubs
  // stub1:
  //                 ldr     x0, ptr1       ; PC-rel load of ptr1
  //                 br      x0             ; Jump to resolver
  // stub2:
  //                 ldr     x0, ptr2       ; PC-rel load of ptr2
  //                 br      x0             ; Jump to resolver
  //
  // ...
  //
  // .section __orc_ptrs
  // ptr1:
  //                 .quad 0x0
  // ptr2:
  //                 .quad 0x0
  //
  // ...

  const unsigned StubSize = IndirectStubsInfo::StubSize;

  // Emit at least MinStubs, rounded up to fill the pages allocated.
  unsigned PageSize = sys::Process::getPageSize();
  unsigned NumPages = ((MinStubs * StubSize) + (PageSize - 1)) / PageSize;
  unsigned NumStubs = (NumPages * PageSize) / StubSize;

  // Allocate memory for stubs and pointers in one call.
  std::error_code EC;
  auto StubsMem = sys::OwningMemoryBlock(sys::Memory::allocateMappedMemory(
      2 * NumPages * PageSize, nullptr,
      sys::Memory::MF_READ | sys::Memory::MF_WRITE, EC));

  if (EC)
    return errorCodeToError(EC);

  // Create separate MemoryBlocks representing the stubs and pointers.
  sys::MemoryBlock StubsBlock(StubsMem.base(), NumPages * PageSize);
  sys::MemoryBlock PtrsBlock(static_cast<char *>(StubsMem.base()) +
                                 NumPages * PageSize,
                             NumPages * PageSize);

  // Populate the stubs page stubs and mark it executable.
  uint64_t *Stub = reinterpret_cast<uint64_t *>(StubsBlock.base());
  uint64_t PtrOffsetField = static_cast<uint64_t>(NumPages * PageSize)
                            << 3;

  for (unsigned I = 0; I < NumStubs; ++I)
    Stub[I] = 0xd61f020058000010 | PtrOffsetField;

  if (auto EC = sys::Memory::protectMappedMemory(
          StubsBlock, sys::Memory::MF_READ | sys::Memory::MF_EXEC))
    return errorCodeToError(EC);

  // Initialize all pointers to point at FailureAddress.
  void **Ptr = reinterpret_cast<void **>(PtrsBlock.base());
  for (unsigned I = 0; I < NumStubs; ++I)
    Ptr[I] = InitialPtrVal;

  StubsInfo = IndirectStubsInfo(NumStubs, std::move(StubsMem));

  return Error::success();
}

void OrcX86_64_Base::writeTrampolines(uint8_t *TrampolineMem,
                                      void *ResolverAddr,
                                      unsigned NumTrampolines) {

  unsigned OffsetToPtr = NumTrampolines * TrampolineSize;

  memcpy(TrampolineMem + OffsetToPtr, &ResolverAddr, sizeof(void *));

  uint64_t *Trampolines = reinterpret_cast<uint64_t *>(TrampolineMem);
  uint64_t CallIndirPCRel = 0xf1c40000000015ff;

  for (unsigned I = 0; I < NumTrampolines; ++I, OffsetToPtr -= TrampolineSize)
    Trampolines[I] = CallIndirPCRel | ((OffsetToPtr - 6) << 16);
}

Error OrcX86_64_Base::emitIndirectStubsBlock(IndirectStubsInfo &StubsInfo,
                                             unsigned MinStubs,
                                             void *InitialPtrVal) {
  // Stub format is:
  //
  // .section __orc_stubs
  // stub1:
  //                 jmpq    *ptr1(%rip)
  //                 .byte   0xC4         ; <- Invalid opcode padding.
  //                 .byte   0xF1
  // stub2:
  //                 jmpq    *ptr2(%rip)
  //
  // ...
  //
  // .section __orc_ptrs
  // ptr1:
  //                 .quad 0x0
  // ptr2:
  //                 .quad 0x0
  //
  // ...

  const unsigned StubSize = IndirectStubsInfo::StubSize;

  // Emit at least MinStubs, rounded up to fill the pages allocated.
  unsigned PageSize = sys::Process::getPageSize();
  unsigned NumPages = ((MinStubs * StubSize) + (PageSize - 1)) / PageSize;
  unsigned NumStubs = (NumPages * PageSize) / StubSize;

  // Allocate memory for stubs and pointers in one call.
  std::error_code EC;
  auto StubsMem = sys::OwningMemoryBlock(sys::Memory::allocateMappedMemory(
      2 * NumPages * PageSize, nullptr,
      sys::Memory::MF_READ | sys::Memory::MF_WRITE, EC));

  if (EC)
    return errorCodeToError(EC);

  // Create separate MemoryBlocks representing the stubs and pointers.
  sys::MemoryBlock StubsBlock(StubsMem.base(), NumPages * PageSize);
  sys::MemoryBlock PtrsBlock(static_cast<char *>(StubsMem.base()) +
                                 NumPages * PageSize,
                             NumPages * PageSize);

  // Populate the stubs page stubs and mark it executable.
  uint64_t *Stub = reinterpret_cast<uint64_t *>(StubsBlock.base());
  uint64_t PtrOffsetField = static_cast<uint64_t>(NumPages * PageSize - 6)
                            << 16;
  for (unsigned I = 0; I < NumStubs; ++I)
    Stub[I] = 0xF1C40000000025ff | PtrOffsetField;

  if (auto EC = sys::Memory::protectMappedMemory(
          StubsBlock, sys::Memory::MF_READ | sys::Memory::MF_EXEC))
    return errorCodeToError(EC);

  // Initialize all pointers to point at FailureAddress.
  void **Ptr = reinterpret_cast<void **>(PtrsBlock.base());
  for (unsigned I = 0; I < NumStubs; ++I)
    Ptr[I] = InitialPtrVal;

  StubsInfo = IndirectStubsInfo(NumStubs, std::move(StubsMem));

  return Error::success();
}

void OrcX86_64_SysV::writeResolverCode(uint8_t *ResolverMem,
                                       JITReentryFn ReentryFn,
                                       void *CallbackMgr) {

  const uint8_t ResolverCode[] = {
      // resolver_entry:
      0x55,                                     // 0x00: pushq     %rbp
      0x48, 0x89, 0xe5,                         // 0x01: movq      %rsp, %rbp
      0x50,                                     // 0x04: pushq     %rax
      0x53,                                     // 0x05: pushq     %rbx
      0x51,                                     // 0x06: pushq     %rcx
      0x52,                                     // 0x07: pushq     %rdx
      0x56,                                     // 0x08: pushq     %rsi
      0x57,                                     // 0x09: pushq     %rdi
      0x41, 0x50,                               // 0x0a: pushq     %r8
      0x41, 0x51,                               // 0x0c: pushq     %r9
      0x41, 0x52,                               // 0x0e: pushq     %r10
      0x41, 0x53,                               // 0x10: pushq     %r11
      0x41, 0x54,                               // 0x12: pushq     %r12
      0x41, 0x55,                               // 0x14: pushq     %r13
      0x41, 0x56,                               // 0x16: pushq     %r14
      0x41, 0x57,                               // 0x18: pushq     %r15
      0x48, 0x81, 0xec, 0x08, 0x02, 0x00, 0x00, // 0x1a: subq      0x208, %rsp
      0x48, 0x0f, 0xae, 0x04, 0x24,             // 0x21: fxsave64  (%rsp)
      0x48, 0xbf,                               // 0x26: movabsq   <CBMgr>, %rdi

      // 0x28: Callback manager addr.
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x48, 0x8b, 0x75, 0x08,                   // 0x30: movq      8(%rbp), %rsi
      0x48, 0x83, 0xee, 0x06,                   // 0x34: subq      $6, %rsi
      0x48, 0xb8,                               // 0x38: movabsq   <REntry>, %rax

      // 0x3a: JIT re-entry fn addr:
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0xff, 0xd0,                               // 0x42: callq     *%rax
      0x48, 0x89, 0x45, 0x08,                   // 0x44: movq      %rax, 8(%rbp)
      0x48, 0x0f, 0xae, 0x0c, 0x24,             // 0x48: fxrstor64 (%rsp)
      0x48, 0x81, 0xc4, 0x08, 0x02, 0x00, 0x00, // 0x4d: addq      0x208, %rsp
      0x41, 0x5f,                               // 0x54: popq      %r15
      0x41, 0x5e,                               // 0x56: popq      %r14
      0x41, 0x5d,                               // 0x58: popq      %r13
      0x41, 0x5c,                               // 0x5a: popq      %r12
      0x41, 0x5b,                               // 0x5c: popq      %r11
      0x41, 0x5a,                               // 0x5e: popq      %r10
      0x41, 0x59,                               // 0x60: popq      %r9
      0x41, 0x58,                               // 0x62: popq      %r8
      0x5f,                                     // 0x64: popq      %rdi
      0x5e,                                     // 0x65: popq      %rsi
      0x5a,                                     // 0x66: popq      %rdx
      0x59,                                     // 0x67: popq      %rcx
      0x5b,                                     // 0x68: popq      %rbx
      0x58,                                     // 0x69: popq      %rax
      0x5d,                                     // 0x6a: popq      %rbp
      0xc3,                                     // 0x6b: retq
  };

  const unsigned ReentryFnAddrOffset = 0x3a;
  const unsigned CallbackMgrAddrOffset = 0x28;

  memcpy(ResolverMem, ResolverCode, sizeof(ResolverCode));
  memcpy(ResolverMem + ReentryFnAddrOffset, &ReentryFn, sizeof(ReentryFn));
  memcpy(ResolverMem + CallbackMgrAddrOffset, &CallbackMgr,
         sizeof(CallbackMgr));
}

void OrcX86_64_Win32::writeResolverCode(uint8_t *ResolverMem,
                                        JITReentryFn ReentryFn,
                                        void *CallbackMgr) {

  // resolverCode is similar to OrcX86_64 with differences specific to windows x64 calling convention:
  // arguments go into rcx, rdx and come in reverse order, shadow space allocation on stack
  const uint8_t ResolverCode[] = {
      // resolver_entry:
      0x55,                                      // 0x00: pushq     %rbp
      0x48, 0x89, 0xe5,                          // 0x01: movq      %rsp, %rbp
      0x50,                                      // 0x04: pushq     %rax
      0x53,                                      // 0x05: pushq     %rbx
      0x51,                                      // 0x06: pushq     %rcx
      0x52,                                      // 0x07: pushq     %rdx
      0x56,                                      // 0x08: pushq     %rsi
      0x57,                                      // 0x09: pushq     %rdi
      0x41, 0x50,                                // 0x0a: pushq     %r8
      0x41, 0x51,                                // 0x0c: pushq     %r9
      0x41, 0x52,                                // 0x0e: pushq     %r10
      0x41, 0x53,                                // 0x10: pushq     %r11
      0x41, 0x54,                                // 0x12: pushq     %r12
      0x41, 0x55,                                // 0x14: pushq     %r13
      0x41, 0x56,                                // 0x16: pushq     %r14
      0x41, 0x57,                                // 0x18: pushq     %r15
      0x48, 0x81, 0xec, 0x08, 0x02, 0x00, 0x00,  // 0x1a: subq      0x208, %rsp
      0x48, 0x0f, 0xae, 0x04, 0x24,              // 0x21: fxsave64  (%rsp)

      0x48, 0xb9,                                // 0x26: movabsq   <CBMgr>, %rcx
      // 0x28: Callback manager addr.
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x48, 0x8B, 0x55, 0x08,                    // 0x30: mov       rdx, [rbp+0x8]
      0x48, 0x83, 0xea, 0x06,                    // 0x34: sub       rdx, 0x6

      0x48, 0xb8,                                // 0x38: movabsq   <REntry>, %rax
      // 0x3a: JIT re-entry fn addr:
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      // 0x42: sub       rsp, 0x20 (Allocate shadow space)
      0x48, 0x83, 0xEC, 0x20,
      0xff, 0xd0,                                // 0x46: callq     *%rax

      // 0x48: add       rsp, 0x20 (Free shadow space)
      0x48, 0x83, 0xC4, 0x20,

      0x48, 0x89, 0x45, 0x08,                    // 0x4C: movq      %rax, 8(%rbp)
      0x48, 0x0f, 0xae, 0x0c, 0x24,              // 0x50: fxrstor64 (%rsp)
      0x48, 0x81, 0xc4, 0x08, 0x02, 0x00, 0x00,  // 0x55: addq      0x208, %rsp
      0x41, 0x5f,                                // 0x5C: popq      %r15
      0x41, 0x5e,                                // 0x5E: popq      %r14
      0x41, 0x5d,                                // 0x60: popq      %r13
      0x41, 0x5c,                                // 0x62: popq      %r12
      0x41, 0x5b,                                // 0x64: popq      %r11
      0x41, 0x5a,                                // 0x66: popq      %r10
      0x41, 0x59,                                // 0x68: popq      %r9
      0x41, 0x58,                                // 0x6a: popq      %r8
      0x5f,                                      // 0x6c: popq      %rdi
      0x5e,                                      // 0x6d: popq      %rsi
      0x5a,                                      // 0x6e: popq      %rdx
      0x59,                                      // 0x6f: popq      %rcx
      0x5b,                                      // 0x70: popq      %rbx
      0x58,                                      // 0x71: popq      %rax
      0x5d,                                      // 0x72: popq      %rbp
      0xc3,                                      // 0x73: retq
  };


  const unsigned ReentryFnAddrOffset = 0x3a;
  const unsigned CallbackMgrAddrOffset = 0x28;

  memcpy(ResolverMem, ResolverCode, sizeof(ResolverCode));
  memcpy(ResolverMem + ReentryFnAddrOffset, &ReentryFn, sizeof(ReentryFn));
  memcpy(ResolverMem + CallbackMgrAddrOffset, &CallbackMgr,
         sizeof(CallbackMgr));
}

void OrcI386::writeResolverCode(uint8_t *ResolverMem, JITReentryFn ReentryFn,
                                void *CallbackMgr) {

  const uint8_t ResolverCode[] = {
      // resolver_entry:
      0x55,                               // 0x00: pushl    %ebp
      0x89, 0xe5,                         // 0x01: movl     %esp, %ebp
      0x54,                               // 0x03: pushl    %esp
      0x83, 0xe4, 0xf0,                   // 0x04: andl     $-0x10, %esp
      0x50,                               // 0x07: pushl    %eax
      0x53,                               // 0x08: pushl    %ebx
      0x51,                               // 0x09: pushl    %ecx
      0x52,                               // 0x0a: pushl    %edx
      0x56,                               // 0x0b: pushl    %esi
      0x57,                               // 0x0c: pushl    %edi
      0x81, 0xec, 0x18, 0x02, 0x00, 0x00, // 0x0d: subl     $0x218, %esp
      0x0f, 0xae, 0x44, 0x24, 0x10,       // 0x13: fxsave   0x10(%esp)
      0x8b, 0x75, 0x04,                   // 0x18: movl     0x4(%ebp), %esi
      0x83, 0xee, 0x05,                   // 0x1b: subl     $0x5, %esi
      0x89, 0x74, 0x24, 0x04,             // 0x1e: movl     %esi, 0x4(%esp)
      0xc7, 0x04, 0x24, 0x00, 0x00, 0x00,
      0x00,                               // 0x22: movl     <cbmgr>, (%esp)
      0xb8, 0x00, 0x00, 0x00, 0x00,       // 0x29: movl     <reentry>, %eax
      0xff, 0xd0,                         // 0x2e: calll    *%eax
      0x89, 0x45, 0x04,                   // 0x30: movl     %eax, 0x4(%ebp)
      0x0f, 0xae, 0x4c, 0x24, 0x10,       // 0x33: fxrstor  0x10(%esp)
      0x81, 0xc4, 0x18, 0x02, 0x00, 0x00, // 0x38: addl     $0x218, %esp
      0x5f,                               // 0x3e: popl     %edi
      0x5e,                               // 0x3f: popl     %esi
      0x5a,                               // 0x40: popl     %edx
      0x59,                               // 0x41: popl     %ecx
      0x5b,                               // 0x42: popl     %ebx
      0x58,                               // 0x43: popl     %eax
      0x8b, 0x65, 0xfc,                   // 0x44: movl     -0x4(%ebp), %esp
      0x5d,                               // 0x48: popl     %ebp
      0xc3                                // 0x49: retl
  };

  const unsigned ReentryFnAddrOffset = 0x2a;
  const unsigned CallbackMgrAddrOffset = 0x25;

  memcpy(ResolverMem, ResolverCode, sizeof(ResolverCode));
  memcpy(ResolverMem + ReentryFnAddrOffset, &ReentryFn, sizeof(ReentryFn));
  memcpy(ResolverMem + CallbackMgrAddrOffset, &CallbackMgr,
         sizeof(CallbackMgr));
}

void OrcI386::writeTrampolines(uint8_t *TrampolineMem, void *ResolverAddr,
                               unsigned NumTrampolines) {

  uint64_t CallRelImm = 0xF1C4C400000000e8;
  uint64_t Resolver = reinterpret_cast<uint64_t>(ResolverAddr);
  uint64_t ResolverRel =
      Resolver - reinterpret_cast<uint64_t>(TrampolineMem) - 5;

  uint64_t *Trampolines = reinterpret_cast<uint64_t *>(TrampolineMem);
  for (unsigned I = 0; I < NumTrampolines; ++I, ResolverRel -= TrampolineSize)
    Trampolines[I] = CallRelImm | (ResolverRel << 8);
}

Error OrcI386::emitIndirectStubsBlock(IndirectStubsInfo &StubsInfo,
                                      unsigned MinStubs, void *InitialPtrVal) {
  // Stub format is:
  //
  // .section __orc_stubs
  // stub1:
  //                 jmpq    *ptr1
  //                 .byte   0xC4         ; <- Invalid opcode padding.
  //                 .byte   0xF1
  // stub2:
  //                 jmpq    *ptr2
  //
  // ...
  //
  // .section __orc_ptrs
  // ptr1:
  //                 .quad 0x0
  // ptr2:
  //                 .quad 0x0
  //
  // ...

  const unsigned StubSize = IndirectStubsInfo::StubSize;

  // Emit at least MinStubs, rounded up to fill the pages allocated.
  unsigned PageSize = sys::Process::getPageSize();
  unsigned NumPages = ((MinStubs * StubSize) + (PageSize - 1)) / PageSize;
  unsigned NumStubs = (NumPages * PageSize) / StubSize;

  // Allocate memory for stubs and pointers in one call.
  std::error_code EC;
  auto StubsMem = sys::OwningMemoryBlock(sys::Memory::allocateMappedMemory(
      2 * NumPages * PageSize, nullptr,
      sys::Memory::MF_READ | sys::Memory::MF_WRITE, EC));

  if (EC)
    return errorCodeToError(EC);

  // Create separate MemoryBlocks representing the stubs and pointers.
  sys::MemoryBlock StubsBlock(StubsMem.base(), NumPages * PageSize);
  sys::MemoryBlock PtrsBlock(static_cast<char *>(StubsMem.base()) +
                                 NumPages * PageSize,
                             NumPages * PageSize);

  // Populate the stubs page stubs and mark it executable.
  uint64_t *Stub = reinterpret_cast<uint64_t *>(StubsBlock.base());
  uint64_t PtrAddr = reinterpret_cast<uint64_t>(PtrsBlock.base());
  for (unsigned I = 0; I < NumStubs; ++I, PtrAddr += 4)
    Stub[I] = 0xF1C40000000025ff | (PtrAddr << 16);

  if (auto EC = sys::Memory::protectMappedMemory(
          StubsBlock, sys::Memory::MF_READ | sys::Memory::MF_EXEC))
    return errorCodeToError(EC);

  // Initialize all pointers to point at FailureAddress.
  void **Ptr = reinterpret_cast<void **>(PtrsBlock.base());
  for (unsigned I = 0; I < NumStubs; ++I)
    Ptr[I] = InitialPtrVal;

  StubsInfo = IndirectStubsInfo(NumStubs, std::move(StubsMem));

  return Error::success();
}

} // End namespace orc.
} // End namespace llvm.
