from PIL import ImageFont, ImageDraw, Image
import re
import sys, getopt, os.path
import codecs
import struct

# 字符 0 -3 7 x 9
start,end = (0x4E00, 0x9FA5)
# start,end = (0x5170, 0x5171)
# with codecs.open("chinese.txt", "wb", encoding="utf-8") as f:
#     for codepoint in range(int(start),int(end)):
#         f.write(chr(codepoint))

fn_bin = "font_16_china" + ".bin"
f_bin = open(fn_bin, 'wb')
f_bin.write(struct.pack('<B', 16))#weite size
f_bin.write(struct.pack('<B', 16))

# fn_txt = "img_lhb" + ".h"
# f_txt = open(fn_txt, 'w')
inc = ""

font = ImageFont.truetype(font='china.ttf', size=16)
pattern = re.compile(r'.{2}')
# im = Image.new('1', (64, 32), (1))
# font_s = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
for codepoint in range(int(start),int(end)):
    font_s = chr(codepoint);
    for j in font_s:
        print (j,end = "")
        im = Image.new("RGBA",(16,16),(255,255,255,0))
        draw = ImageDraw.Draw(im)
        draw.text((-1, -2), j, font=font, fill = 'black')
        # im.show()
        # inc = '';
        # inc += "/*-- " + j + " --*/\n";
        for h in range(16):
            for w in range(16):
                r =  im.getpixel((w, h))[0] >> 3
                g =  im.getpixel((w, h))[1] >> 2
                b =  im.getpixel((w, h))[2] >> 3
                px_out = (r << 11) + (g << 5) + b
                f_bin.write(struct.pack('<B', im.getpixel((w, h))[3]))
                # print ( '0x' + "%x"%im.getpixel((w, h))[3] + ',', end=' ' )
                inc += '0x' + "%x"%im.getpixel((w, h))[3] + ',';
            # print('')
            inc += "\n";
    # f_txt.write(inc)
# f_txt.close
f_bin.close()

