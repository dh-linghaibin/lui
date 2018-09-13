from PIL import ImageFont, ImageDraw, Image
import re
import sys, getopt, os.path
import codecs
import struct

name = 'battery.png'

im=Image.open(name)

weigth = im.size[0]#320 #图片
length = im.size[1]#240 #图片长度
type   = 2   #1 jpg 2 png

fn_bin = name + ".bin"
f_bin = open(fn_bin, 'wb')
f_bin.write(struct.pack('<B', type)) #type
f_bin.write(struct.pack('<H', weigth)) #weite size
f_bin.write(struct.pack('<H', length))

fn_txt = "board" + ".h"
f_txt = open(fn_txt, 'w')

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
        txt_c16 += str(px_out_l) + ", " + str(px_out_h) + ", "
        if type == 1 :
            f_bin.write(struct.pack('<H', px_out))
        elif type == 2 :
            f_bin.write(struct.pack('<H', px_out))
            f_bin.write(struct.pack('<B', im.getpixel((w, h))[3]))
    txt_c16 += "\n"
    f_txt.write(txt_c16)
f_txt.close()
f_bin.close()
