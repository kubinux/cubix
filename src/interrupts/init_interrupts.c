// Copyright (C) 2014 Jakub Lewandowski <jakub.lewandowski@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <interrupts/init_interrupts.h>
#include <interrupts/gate_descriptor.h>
#include <interrupts/isr.h>
#include <io/port_io.h>
#include <stdint.h>


static struct gate_descriptor idt[256];


static struct idt_register
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_register = { sizeof(idt) - 1, (uint64_t)idt };


static void init_idt(void)
{
    init_trap_gate(idt + 0, (uintptr_t)isr_entry_0);
    init_trap_gate(idt + 1, (uintptr_t)isr_entry_1);
    init_trap_gate(idt + 2, (uintptr_t)isr_entry_2);
    init_trap_gate(idt + 3, (uintptr_t)isr_entry_3);
    init_trap_gate(idt + 4, (uintptr_t)isr_entry_4);
    init_trap_gate(idt + 5, (uintptr_t)isr_entry_5);
    init_trap_gate(idt + 6, (uintptr_t)isr_entry_6);
    init_trap_gate(idt + 7, (uintptr_t)isr_entry_7);
    init_trap_gate(idt + 8, (uintptr_t)isr_entry_8);
    init_trap_gate(idt + 9, (uintptr_t)isr_entry_9);
    init_trap_gate(idt + 10, (uintptr_t)isr_entry_10);
    init_trap_gate(idt + 11, (uintptr_t)isr_entry_11);
    init_trap_gate(idt + 12, (uintptr_t)isr_entry_12);
    init_trap_gate(idt + 13, (uintptr_t)isr_entry_13);
    init_trap_gate(idt + 14, (uintptr_t)isr_entry_14);
    init_trap_gate(idt + 15, (uintptr_t)isr_entry_15);
    init_trap_gate(idt + 16, (uintptr_t)isr_entry_16);
    init_trap_gate(idt + 17, (uintptr_t)isr_entry_17);
    init_trap_gate(idt + 18, (uintptr_t)isr_entry_18);
    init_trap_gate(idt + 19, (uintptr_t)isr_entry_19);
    init_trap_gate(idt + 20, (uintptr_t)isr_entry_20);
    init_trap_gate(idt + 21, (uintptr_t)isr_entry_21);
    init_trap_gate(idt + 22, (uintptr_t)isr_entry_22);
    init_trap_gate(idt + 23, (uintptr_t)isr_entry_23);
    init_trap_gate(idt + 24, (uintptr_t)isr_entry_24);
    init_trap_gate(idt + 25, (uintptr_t)isr_entry_25);
    init_trap_gate(idt + 26, (uintptr_t)isr_entry_26);
    init_trap_gate(idt + 27, (uintptr_t)isr_entry_27);
    init_trap_gate(idt + 28, (uintptr_t)isr_entry_28);
    init_trap_gate(idt + 29, (uintptr_t)isr_entry_29);
    init_trap_gate(idt + 30, (uintptr_t)isr_entry_30);
    init_trap_gate(idt + 31, (uintptr_t)isr_entry_31);
    init_interrupt_gate(idt + 32, (uintptr_t)isr_entry_32);
    init_interrupt_gate(idt + 33, (uintptr_t)isr_entry_33);
    init_interrupt_gate(idt + 34, (uintptr_t)isr_entry_34);
    init_interrupt_gate(idt + 35, (uintptr_t)isr_entry_35);
    init_interrupt_gate(idt + 36, (uintptr_t)isr_entry_36);
    init_interrupt_gate(idt + 37, (uintptr_t)isr_entry_37);
    init_interrupt_gate(idt + 38, (uintptr_t)isr_entry_38);
    init_interrupt_gate(idt + 39, (uintptr_t)isr_entry_39);
    init_interrupt_gate(idt + 40, (uintptr_t)isr_entry_40);
    init_interrupt_gate(idt + 41, (uintptr_t)isr_entry_41);
    init_interrupt_gate(idt + 42, (uintptr_t)isr_entry_42);
    init_interrupt_gate(idt + 43, (uintptr_t)isr_entry_43);
    init_interrupt_gate(idt + 44, (uintptr_t)isr_entry_44);
    init_interrupt_gate(idt + 45, (uintptr_t)isr_entry_45);
    init_interrupt_gate(idt + 46, (uintptr_t)isr_entry_46);
    init_interrupt_gate(idt + 47, (uintptr_t)isr_entry_47);
    init_interrupt_gate(idt + 48, (uintptr_t)isr_entry_48);
    init_interrupt_gate(idt + 49, (uintptr_t)isr_entry_49);
    init_interrupt_gate(idt + 50, (uintptr_t)isr_entry_50);
    init_interrupt_gate(idt + 51, (uintptr_t)isr_entry_51);
    init_interrupt_gate(idt + 52, (uintptr_t)isr_entry_52);
    init_interrupt_gate(idt + 53, (uintptr_t)isr_entry_53);
    init_interrupt_gate(idt + 54, (uintptr_t)isr_entry_54);
    init_interrupt_gate(idt + 55, (uintptr_t)isr_entry_55);
    init_interrupt_gate(idt + 56, (uintptr_t)isr_entry_56);
    init_interrupt_gate(idt + 57, (uintptr_t)isr_entry_57);
    init_interrupt_gate(idt + 58, (uintptr_t)isr_entry_58);
    init_interrupt_gate(idt + 59, (uintptr_t)isr_entry_59);
    init_interrupt_gate(idt + 60, (uintptr_t)isr_entry_60);
    init_interrupt_gate(idt + 61, (uintptr_t)isr_entry_61);
    init_interrupt_gate(idt + 62, (uintptr_t)isr_entry_62);
    init_interrupt_gate(idt + 63, (uintptr_t)isr_entry_63);
    init_interrupt_gate(idt + 64, (uintptr_t)isr_entry_64);
    init_interrupt_gate(idt + 65, (uintptr_t)isr_entry_65);
    init_interrupt_gate(idt + 66, (uintptr_t)isr_entry_66);
    init_interrupt_gate(idt + 67, (uintptr_t)isr_entry_67);
    init_interrupt_gate(idt + 68, (uintptr_t)isr_entry_68);
    init_interrupt_gate(idt + 69, (uintptr_t)isr_entry_69);
    init_interrupt_gate(idt + 70, (uintptr_t)isr_entry_70);
    init_interrupt_gate(idt + 71, (uintptr_t)isr_entry_71);
    init_interrupt_gate(idt + 72, (uintptr_t)isr_entry_72);
    init_interrupt_gate(idt + 73, (uintptr_t)isr_entry_73);
    init_interrupt_gate(idt + 74, (uintptr_t)isr_entry_74);
    init_interrupt_gate(idt + 75, (uintptr_t)isr_entry_75);
    init_interrupt_gate(idt + 76, (uintptr_t)isr_entry_76);
    init_interrupt_gate(idt + 77, (uintptr_t)isr_entry_77);
    init_interrupt_gate(idt + 78, (uintptr_t)isr_entry_78);
    init_interrupt_gate(idt + 79, (uintptr_t)isr_entry_79);
    init_interrupt_gate(idt + 80, (uintptr_t)isr_entry_80);
    init_interrupt_gate(idt + 81, (uintptr_t)isr_entry_81);
    init_interrupt_gate(idt + 82, (uintptr_t)isr_entry_82);
    init_interrupt_gate(idt + 83, (uintptr_t)isr_entry_83);
    init_interrupt_gate(idt + 84, (uintptr_t)isr_entry_84);
    init_interrupt_gate(idt + 85, (uintptr_t)isr_entry_85);
    init_interrupt_gate(idt + 86, (uintptr_t)isr_entry_86);
    init_interrupt_gate(idt + 87, (uintptr_t)isr_entry_87);
    init_interrupt_gate(idt + 88, (uintptr_t)isr_entry_88);
    init_interrupt_gate(idt + 89, (uintptr_t)isr_entry_89);
    init_interrupt_gate(idt + 90, (uintptr_t)isr_entry_90);
    init_interrupt_gate(idt + 91, (uintptr_t)isr_entry_91);
    init_interrupt_gate(idt + 92, (uintptr_t)isr_entry_92);
    init_interrupt_gate(idt + 93, (uintptr_t)isr_entry_93);
    init_interrupt_gate(idt + 94, (uintptr_t)isr_entry_94);
    init_interrupt_gate(idt + 95, (uintptr_t)isr_entry_95);
    init_interrupt_gate(idt + 96, (uintptr_t)isr_entry_96);
    init_interrupt_gate(idt + 97, (uintptr_t)isr_entry_97);
    init_interrupt_gate(idt + 98, (uintptr_t)isr_entry_98);
    init_interrupt_gate(idt + 99, (uintptr_t)isr_entry_99);
    init_interrupt_gate(idt + 100, (uintptr_t)isr_entry_100);
    init_interrupt_gate(idt + 101, (uintptr_t)isr_entry_101);
    init_interrupt_gate(idt + 102, (uintptr_t)isr_entry_102);
    init_interrupt_gate(idt + 103, (uintptr_t)isr_entry_103);
    init_interrupt_gate(idt + 104, (uintptr_t)isr_entry_104);
    init_interrupt_gate(idt + 105, (uintptr_t)isr_entry_105);
    init_interrupt_gate(idt + 106, (uintptr_t)isr_entry_106);
    init_interrupt_gate(idt + 107, (uintptr_t)isr_entry_107);
    init_interrupt_gate(idt + 108, (uintptr_t)isr_entry_108);
    init_interrupt_gate(idt + 109, (uintptr_t)isr_entry_109);
    init_interrupt_gate(idt + 110, (uintptr_t)isr_entry_110);
    init_interrupt_gate(idt + 111, (uintptr_t)isr_entry_111);
    init_interrupt_gate(idt + 112, (uintptr_t)isr_entry_112);
    init_interrupt_gate(idt + 113, (uintptr_t)isr_entry_113);
    init_interrupt_gate(idt + 114, (uintptr_t)isr_entry_114);
    init_interrupt_gate(idt + 115, (uintptr_t)isr_entry_115);
    init_interrupt_gate(idt + 116, (uintptr_t)isr_entry_116);
    init_interrupt_gate(idt + 117, (uintptr_t)isr_entry_117);
    init_interrupt_gate(idt + 118, (uintptr_t)isr_entry_118);
    init_interrupt_gate(idt + 119, (uintptr_t)isr_entry_119);
    init_interrupt_gate(idt + 120, (uintptr_t)isr_entry_120);
    init_interrupt_gate(idt + 121, (uintptr_t)isr_entry_121);
    init_interrupt_gate(idt + 122, (uintptr_t)isr_entry_122);
    init_interrupt_gate(idt + 123, (uintptr_t)isr_entry_123);
    init_interrupt_gate(idt + 124, (uintptr_t)isr_entry_124);
    init_interrupt_gate(idt + 125, (uintptr_t)isr_entry_125);
    init_interrupt_gate(idt + 126, (uintptr_t)isr_entry_126);
    init_interrupt_gate(idt + 127, (uintptr_t)isr_entry_127);
    init_interrupt_gate(idt + 128, (uintptr_t)isr_entry_128);
    init_interrupt_gate(idt + 129, (uintptr_t)isr_entry_129);
    init_interrupt_gate(idt + 130, (uintptr_t)isr_entry_130);
    init_interrupt_gate(idt + 131, (uintptr_t)isr_entry_131);
    init_interrupt_gate(idt + 132, (uintptr_t)isr_entry_132);
    init_interrupt_gate(idt + 133, (uintptr_t)isr_entry_133);
    init_interrupt_gate(idt + 134, (uintptr_t)isr_entry_134);
    init_interrupt_gate(idt + 135, (uintptr_t)isr_entry_135);
    init_interrupt_gate(idt + 136, (uintptr_t)isr_entry_136);
    init_interrupt_gate(idt + 137, (uintptr_t)isr_entry_137);
    init_interrupt_gate(idt + 138, (uintptr_t)isr_entry_138);
    init_interrupt_gate(idt + 139, (uintptr_t)isr_entry_139);
    init_interrupt_gate(idt + 140, (uintptr_t)isr_entry_140);
    init_interrupt_gate(idt + 141, (uintptr_t)isr_entry_141);
    init_interrupt_gate(idt + 142, (uintptr_t)isr_entry_142);
    init_interrupt_gate(idt + 143, (uintptr_t)isr_entry_143);
    init_interrupt_gate(idt + 144, (uintptr_t)isr_entry_144);
    init_interrupt_gate(idt + 145, (uintptr_t)isr_entry_145);
    init_interrupt_gate(idt + 146, (uintptr_t)isr_entry_146);
    init_interrupt_gate(idt + 147, (uintptr_t)isr_entry_147);
    init_interrupt_gate(idt + 148, (uintptr_t)isr_entry_148);
    init_interrupt_gate(idt + 149, (uintptr_t)isr_entry_149);
    init_interrupt_gate(idt + 150, (uintptr_t)isr_entry_150);
    init_interrupt_gate(idt + 151, (uintptr_t)isr_entry_151);
    init_interrupt_gate(idt + 152, (uintptr_t)isr_entry_152);
    init_interrupt_gate(idt + 153, (uintptr_t)isr_entry_153);
    init_interrupt_gate(idt + 154, (uintptr_t)isr_entry_154);
    init_interrupt_gate(idt + 155, (uintptr_t)isr_entry_155);
    init_interrupt_gate(idt + 156, (uintptr_t)isr_entry_156);
    init_interrupt_gate(idt + 157, (uintptr_t)isr_entry_157);
    init_interrupt_gate(idt + 158, (uintptr_t)isr_entry_158);
    init_interrupt_gate(idt + 159, (uintptr_t)isr_entry_159);
    init_interrupt_gate(idt + 160, (uintptr_t)isr_entry_160);
    init_interrupt_gate(idt + 161, (uintptr_t)isr_entry_161);
    init_interrupt_gate(idt + 162, (uintptr_t)isr_entry_162);
    init_interrupt_gate(idt + 163, (uintptr_t)isr_entry_163);
    init_interrupt_gate(idt + 164, (uintptr_t)isr_entry_164);
    init_interrupt_gate(idt + 165, (uintptr_t)isr_entry_165);
    init_interrupt_gate(idt + 166, (uintptr_t)isr_entry_166);
    init_interrupt_gate(idt + 167, (uintptr_t)isr_entry_167);
    init_interrupt_gate(idt + 168, (uintptr_t)isr_entry_168);
    init_interrupt_gate(idt + 169, (uintptr_t)isr_entry_169);
    init_interrupt_gate(idt + 170, (uintptr_t)isr_entry_170);
    init_interrupt_gate(idt + 171, (uintptr_t)isr_entry_171);
    init_interrupt_gate(idt + 172, (uintptr_t)isr_entry_172);
    init_interrupt_gate(idt + 173, (uintptr_t)isr_entry_173);
    init_interrupt_gate(idt + 174, (uintptr_t)isr_entry_174);
    init_interrupt_gate(idt + 175, (uintptr_t)isr_entry_175);
    init_interrupt_gate(idt + 176, (uintptr_t)isr_entry_176);
    init_interrupt_gate(idt + 177, (uintptr_t)isr_entry_177);
    init_interrupt_gate(idt + 178, (uintptr_t)isr_entry_178);
    init_interrupt_gate(idt + 179, (uintptr_t)isr_entry_179);
    init_interrupt_gate(idt + 180, (uintptr_t)isr_entry_180);
    init_interrupt_gate(idt + 181, (uintptr_t)isr_entry_181);
    init_interrupt_gate(idt + 182, (uintptr_t)isr_entry_182);
    init_interrupt_gate(idt + 183, (uintptr_t)isr_entry_183);
    init_interrupt_gate(idt + 184, (uintptr_t)isr_entry_184);
    init_interrupt_gate(idt + 185, (uintptr_t)isr_entry_185);
    init_interrupt_gate(idt + 186, (uintptr_t)isr_entry_186);
    init_interrupt_gate(idt + 187, (uintptr_t)isr_entry_187);
    init_interrupt_gate(idt + 188, (uintptr_t)isr_entry_188);
    init_interrupt_gate(idt + 189, (uintptr_t)isr_entry_189);
    init_interrupt_gate(idt + 190, (uintptr_t)isr_entry_190);
    init_interrupt_gate(idt + 191, (uintptr_t)isr_entry_191);
    init_interrupt_gate(idt + 192, (uintptr_t)isr_entry_192);
    init_interrupt_gate(idt + 193, (uintptr_t)isr_entry_193);
    init_interrupt_gate(idt + 194, (uintptr_t)isr_entry_194);
    init_interrupt_gate(idt + 195, (uintptr_t)isr_entry_195);
    init_interrupt_gate(idt + 196, (uintptr_t)isr_entry_196);
    init_interrupt_gate(idt + 197, (uintptr_t)isr_entry_197);
    init_interrupt_gate(idt + 198, (uintptr_t)isr_entry_198);
    init_interrupt_gate(idt + 199, (uintptr_t)isr_entry_199);
    init_interrupt_gate(idt + 200, (uintptr_t)isr_entry_200);
    init_interrupt_gate(idt + 201, (uintptr_t)isr_entry_201);
    init_interrupt_gate(idt + 202, (uintptr_t)isr_entry_202);
    init_interrupt_gate(idt + 203, (uintptr_t)isr_entry_203);
    init_interrupt_gate(idt + 204, (uintptr_t)isr_entry_204);
    init_interrupt_gate(idt + 205, (uintptr_t)isr_entry_205);
    init_interrupt_gate(idt + 206, (uintptr_t)isr_entry_206);
    init_interrupt_gate(idt + 207, (uintptr_t)isr_entry_207);
    init_interrupt_gate(idt + 208, (uintptr_t)isr_entry_208);
    init_interrupt_gate(idt + 209, (uintptr_t)isr_entry_209);
    init_interrupt_gate(idt + 210, (uintptr_t)isr_entry_210);
    init_interrupt_gate(idt + 211, (uintptr_t)isr_entry_211);
    init_interrupt_gate(idt + 212, (uintptr_t)isr_entry_212);
    init_interrupt_gate(idt + 213, (uintptr_t)isr_entry_213);
    init_interrupt_gate(idt + 214, (uintptr_t)isr_entry_214);
    init_interrupt_gate(idt + 215, (uintptr_t)isr_entry_215);
    init_interrupt_gate(idt + 216, (uintptr_t)isr_entry_216);
    init_interrupt_gate(idt + 217, (uintptr_t)isr_entry_217);
    init_interrupt_gate(idt + 218, (uintptr_t)isr_entry_218);
    init_interrupt_gate(idt + 219, (uintptr_t)isr_entry_219);
    init_interrupt_gate(idt + 220, (uintptr_t)isr_entry_220);
    init_interrupt_gate(idt + 221, (uintptr_t)isr_entry_221);
    init_interrupt_gate(idt + 222, (uintptr_t)isr_entry_222);
    init_interrupt_gate(idt + 223, (uintptr_t)isr_entry_223);
    init_interrupt_gate(idt + 224, (uintptr_t)isr_entry_224);
    init_interrupt_gate(idt + 225, (uintptr_t)isr_entry_225);
    init_interrupt_gate(idt + 226, (uintptr_t)isr_entry_226);
    init_interrupt_gate(idt + 227, (uintptr_t)isr_entry_227);
    init_interrupt_gate(idt + 228, (uintptr_t)isr_entry_228);
    init_interrupt_gate(idt + 229, (uintptr_t)isr_entry_229);
    init_interrupt_gate(idt + 230, (uintptr_t)isr_entry_230);
    init_interrupt_gate(idt + 231, (uintptr_t)isr_entry_231);
    init_interrupt_gate(idt + 232, (uintptr_t)isr_entry_232);
    init_interrupt_gate(idt + 233, (uintptr_t)isr_entry_233);
    init_interrupt_gate(idt + 234, (uintptr_t)isr_entry_234);
    init_interrupt_gate(idt + 235, (uintptr_t)isr_entry_235);
    init_interrupt_gate(idt + 236, (uintptr_t)isr_entry_236);
    init_interrupt_gate(idt + 237, (uintptr_t)isr_entry_237);
    init_interrupt_gate(idt + 238, (uintptr_t)isr_entry_238);
    init_interrupt_gate(idt + 239, (uintptr_t)isr_entry_239);
    init_interrupt_gate(idt + 240, (uintptr_t)isr_entry_240);
    init_interrupt_gate(idt + 241, (uintptr_t)isr_entry_241);
    init_interrupt_gate(idt + 242, (uintptr_t)isr_entry_242);
    init_interrupt_gate(idt + 243, (uintptr_t)isr_entry_243);
    init_interrupt_gate(idt + 244, (uintptr_t)isr_entry_244);
    init_interrupt_gate(idt + 245, (uintptr_t)isr_entry_245);
    init_interrupt_gate(idt + 246, (uintptr_t)isr_entry_246);
    init_interrupt_gate(idt + 247, (uintptr_t)isr_entry_247);
    init_interrupt_gate(idt + 248, (uintptr_t)isr_entry_248);
    init_interrupt_gate(idt + 249, (uintptr_t)isr_entry_249);
    init_interrupt_gate(idt + 250, (uintptr_t)isr_entry_250);
    init_interrupt_gate(idt + 251, (uintptr_t)isr_entry_251);
    init_interrupt_gate(idt + 252, (uintptr_t)isr_entry_252);
    init_interrupt_gate(idt + 253, (uintptr_t)isr_entry_253);
    init_interrupt_gate(idt + 254, (uintptr_t)isr_entry_254);
    init_interrupt_gate(idt + 255, (uintptr_t)isr_entry_255);
}


void init_interrupts(void)
{
    init_idt();

    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    __asm__ __volatile__("lidt (%0)"::"r" (&idt_register));
    __asm__ __volatile__("sti");
}



