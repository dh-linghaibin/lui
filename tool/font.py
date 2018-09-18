from PIL import ImageFont, ImageDraw, Image
import re
import sys, getopt, os.path
import codecs
import struct

we = 16
wh = 16
type = 1;
name = "china_16"

# 字符 0 -3 7 x 9
start,end = (0x4E00, 0x9FA5)
# start,end = (0x5170, 0x5171)
# with codecs.open("chinese.txt", "wb", encoding="utf-8") as f:
#     for codepoint in range(int(start),int(end)):
#         f.write(chr(codepoint))

fn_bin = name + ".bin"
f_bin = open(fn_bin, 'wb')
f_bin.write(struct.pack('<B', type))
f_bin.write(struct.pack('<H', we))#weite size
f_bin.write(struct.pack('<H', wh))

font = ImageFont.truetype(font='china.ttf', size=16)

font_s = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
for j in font_s:
    print (j,end = "")
    im = Image.new("RGBA",(we,wh),(255,255,255,0))
    draw = ImageDraw.Draw(im)
    draw.text((-1, -2), j, font=font, fill = 'black')
    for h in range(wh):
        for w in range(we):
            f_bin.write(struct.pack('<B', im.getpixel((w, h))[3]))

pattern = re.compile(r'.{2}')
for codepoint in range(int(start),int(end)):
    font_s = chr(codepoint);
    for j in font_s:
        print (j,end = "")
        im = Image.new("RGBA",(we,wh),(255,255,255,0))
        draw = ImageDraw.Draw(im)
        draw.text((-1, -2), j, font=font, fill = 'black')
        # im.show()
        for h in range(wh):
            for w in range(we):
                # r =  im.getpixel((w, h))[0] >> 3
                # g =  im.getpixel((w, h))[1] >> 2
                # b =  im.getpixel((w, h))[2] >> 3
                # px_out = (r << 11) + (g << 5) + b
                f_bin.write(struct.pack('<B', im.getpixel((w, h))[3]))
f_bin.close()

