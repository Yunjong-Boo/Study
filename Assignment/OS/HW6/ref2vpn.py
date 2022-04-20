refFile = open('./ls_mem.txt', 'r')
vpnFile = open('./vpn.txt', 'w')

for line in refFile:
        vpnFile.write(str((int("0x" + line[3:11], 16) & 0xfffff000) >> 12) + "\n")

refFile.close()
vpnFile.close()
