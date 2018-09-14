from PIL import ImageFont, ImageDraw, Image
import re
import sys, getopt, os.path
import codecs
import struct

# 字符 0 -3 7 x 9
we = 20
wh = 20
type = 1;
fn_txt = "china_tft_32" + ".h"
fn_base = "china_tft_32"
f_txt = open(fn_txt, 'w')
inc = "#include <stdint.h> \r\n"
f_txt.write(inc)
f_txt.write("const uint8_t " + fn_base + "[] = {")
inc = ""

f_txt.write('0x' + "%x"%type + ',') #type
f_txt.write('0x' + "%x"%we + ',')#weite size
f_txt.write('0x' + "%x"%wh + ',')

font = ImageFont.truetype(font='china.ttf', size=30)
font_s = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
# font_s = "8"
for j in font_s:
    print (j,end = "")
    im = Image.new("RGBA",(we,wh),(255,255,255,0))
    draw = ImageDraw.Draw(im)
    draw.text((0, -10), j, font=font, fill = 'black')
    # im.show()
    inc = '';
    inc += "/*-- " + j + " --*/\n";
    for h in range(wh):
        for w in range(we):
            inc += '0x' + "%x"%im.getpixel((w, h))[3] + ',';
        inc += "\n";
    f_txt.write(inc)
f_txt.write("};  /*Arrray end*/")
f_txt.close

