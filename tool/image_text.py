from PIL import ImageFont, ImageDraw, Image
import re
import sys, getopt, os.path
import codecs
import struct

name = 'desk.jpg'

im=Image.open(name)

weigth = im.size[0]#320 #图片
length = im.size[1]#240 #图片长度
type   = 1   #1 jpg 2 png

fn_txt = name.replace(".", "_") + ".h"
f_txt = open(fn_txt, 'w')
f_txt.write("#include <stdint.h> \r\n")
f_txt.write("const uint8_t " + name.replace(".", "_") + "[] = {")
f_txt.write('0x' + "%x"%type + ',') #type
f_txt.write('0x' + "%x"%(weigth & 0x00ff) + ',' + '0x' + "%x"%(weigth >> 8) + ',') #weite size
f_txt.write('0x' + "%x"%(length & 0x00ff) + ',' + '0x' + "%x"%(length >> 8) + ',')
f_txt.write("/*-- val --*/\n")

txt_c16 = ''
# print(im.size)
# im.show()
for h in range(length):
    txt_c16 = ''
    for w in range(weigth):
        # 16 bit colors
        r =  im.getpixel((w, h))[0] >> 3
        g =  im.getpixel((w, h))[1] >> 2
        b =  im.getpixel((w, h))[2] >> 3
        px_out = (r << 11) + (g << 5) + b
        px_out_h = px_out >> 8
        px_out_l = px_out & 0x00ff
        if type == 1 :
            txt_c16 += str(px_out_l) + ", " + str(px_out_h) + ", "
        elif type == 2 :
            txt_c16 += str(px_out_l) + ", " + str(px_out_h) + ", "
            txt_c16 += str(im.getpixel((w, h))[3]) + ", "
    txt_c16 += "\n"
    f_txt.write(txt_c16)
f_txt.write("};  /*Arrray end*/")
f_txt.close()
