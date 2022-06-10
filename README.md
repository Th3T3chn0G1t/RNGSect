# Generating an "OS" from RNG
### Possibly the dumbest thing I've ever done

![](screenshots/generated.png)

So my friend Marca and I were joking about shredding drives using `dd if=/dev/random` when he profferred the opening to a rabbit hole:
![](screenshots/marca.png)

Now obviously generating a "completely functional" OS is out the window - being too subjective and also basically impossible.

But if we narrow the definition of "functional OS" to "something that boots and doesn't crash" (as we did) - then you have a challenge.

Marca elected to go down the route of "simulation" - not quite writing an 8086 emulator but implementing enough to verify whether a boot sector met the criteria. This is in principle a more rigourous test as it means that possibly valid operations like memory operations or division can be checked instead of just discarded - at the downside of being a significantly larger endevour.

I decided on the less thorough but still effective approach of going through the 8086's manual and finding opcodes which could not error out. This involved understanding which parts of encodings represented memory vs. register operations and checking the stride of each encoding (i.e. how many 1-byte fetches it would trigger on a real CPU).

The result of my primary endevour was the `nops.txt` file enclosed. This file represents all bit patterns of opcodes whose effects cannot crash the execution, and ferry the IP toward an eventual `hlt`. Then it was simply a case of converting these bit patterns and wildcards into a ludicrous switch statement intermixed with bitwise+logical ops and setting the stride of each instruction (Thank you VLE, very cool).

Then it is simply a case of generating a random sector, checking offsets and jumping by stride, checking for boot magic and that the last instruction after the effective-nops was `hlt`. Much to my joy, it worked the first time I ran it and produced the output displayed at the top of this file! With a few runs' experience it seems to work fine, though I would be remiss to consider a program which relies on random input to be flawless.

If you want to try yourself, all you need to do is run `make` and the program will be compiled, perform generation and create a disassembly, and run the result with qemu. In my experience the whole process takes around ~30 seconds on average (obviously the nature of RNG means this can fluctuate drastically but 30 seems to be a ballpark average) on my 2012 macbook - your mileage may vary.

At some point I may revisit this project and try to find boot sectors which produce output in addition to doing nothing - such as via. serial or by writing to the VGA buffer. For the time being however, it persists as solely a fun oddity in my Github profile.
