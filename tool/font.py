from struct import *
from PIL import ImageFont, ImageDraw, Image
import re

font = ImageFont.truetype(font='test2.ttf', size=20)
pattern = re.compile(r'.{2}')
# im = Image.new('1', (64, 32), (1))
im = Image.new("RGBA",(16,16),(255,255,255,0))
draw = ImageDraw.Draw(im)
draw.text((0, 1), '1', font=font, fill = 'black')

# im.show()

res = []
for h in range(16):
    tmp = ''
    for w in range(16):
        r =  im.getpixel((w, h))[0] >> 3
        g =  im.getpixel((w, h))[1] >> 2
        b =  im.getpixel((w, h))[2] >> 3
        px_out = (r << 11) + (g << 5) + b
        print ( '0x' + "%x"%px_out + ',' + '0x' + "%x"%im.getpixel((w, h))[3] + ',', end=' ' )
        if px_out != 65535:
            tmp += '0'
            # print(' ', end='')
        else:
            tmp += '1'
            # print('●', end='')
    print('')
    res.append(','.join(map(lambda x: '0x' + x, pattern.findall("%016X" % int(tmp, 2)))) + ',')
res = ''.join(res)[:-1]
print(res)