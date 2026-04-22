this project is made in logisim-evolution 4.0, please use 4.0 or later to run


this is the first prototype of the first cpu in this line-up, it does not have all of its features yet and
still has some hardware bugs.

bugs that i know of at the moment:

  if this sequence enters the pipeline
    alu -> mem -> mem
    where alu is an alu operation and mem is a memory operation,
    the second mem is not executed, to solve this temporarily just
    add a nop between them
    mem -> mem works fine outside this context

  if a software interrupt has already entered the pipeline and a hardware
    interrupt is triggered, it will jump to the hardware address as its
    higher priority but it will push to the interrupt stack both addresses.
    first the software address and then the hardware. might be turned into a 
    feature if it seems useful


missing features:
  load imm with 10bit address, 12 data bits instruction
  mov user regs with both parameters as addresses
  timers
  priviledge mode
  executing/accessing priviledge while in user mode interrupt
  cpu info register
  full implementation of the status and control registers
