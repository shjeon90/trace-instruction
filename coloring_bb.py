import sark
import os

def check_bb(bbs, bb):
    for bb_ in bbs:
        if bb_ == bb: return True
    return False

def set_color(path, base):

    with open(path, 'r') as f:
        lines = f.read()
    list_ins = lines.split('\n')

    base_addr = 0
    if base:
        base_addr = list_ins[0]
        print base_addr
        base_addr = base_addr.split(' ')[2]
        base_addr = int(base_addr, 16)

    basic_blocks = []

    for ins in list_ins[1:]:
        if len(ins) == 0: continue
        addr_ins = ins.split(' ', 1)
        addr_ins[1] = addr_ins[1].strip()
        addr = int(addr_ins[0], 16) - base_addr

        basic_block = sark.CodeBlock(addr)
        if not check_bb(basic_blocks, basic_block):
            basic_blocks.append(basic_block)
            basic_block.color = 0x00ff0000

def main(file, base):
    if not os.path.exists(file):
        raise Exception('No such file.')

    set_color(file, base)

if __name__ == '__main__':
    '''
    Example of instruction file
    
    Image base: 0x55a24d44c000
    0x55a24d44d0e0 nop edx, edi
    0x55a24d44d0e4 xor ebp, ebp
    0x55a24d44d0e6 mov r9, rdx
    0x55a24d44d0e9 pop rsi
    0x55a24d44d0ea mov rdx, rsp
    0x55a24d44d0ed and rsp, 0xfffffffffffffff0
    '''

    file = 'path/to/instructions'
    base = True

    main(file, base)