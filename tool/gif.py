#-*- coding: UTF-8 -*-  
 
import os
from PIL import Image
import struct

def analyseImage(path):
    '''
    Pre-process pass over the image to determine the mode (full or additive).
    Necessary as assessing single frames isn't reliable. Need to know the mode 
    before processing all frames.
    '''
    im = Image.open(path)
    results = {
        'size': im.size,
        'mode': 'full',
    }
    try:
        while True:
            if im.tile:
                tile = im.tile[0]
                update_region = tile[1]
                update_region_dimensions = update_region[2:]
                if update_region_dimensions != im.size:
                    results['mode'] = 'partial'
                    break
            im.seek(im.tell() + 1)
    except EOFError:
        pass
    return results
 
 
def processImage(path):
    '''
    Iterate the GIF, extracting each frame.
    '''
    mode = analyseImage(path)['mode']
    im = Image.open(path)
    p = im.getpalette()

    i = 0
    try:
        while True:
            print ("saving %s (%s) frame %d, %s %s" % (path, mode, i, im.size, im.tile))
            '''
            If the GIF uses local colour tables, each frame will have its own palette.
            If not, we need to apply the global palette to the new frame.
            '''
            if not im.getpalette():
                im.putpalette(p)
            # last_frame = im.convert('RGBA')
            # last_frame.show()
            '''
            Is this file a "partial"-mode GIF where frames update a region of a different size to the entire image?
            If so, we need to construct the new frame by pasting it on top of the preceding frames.
            '''
            i += 1
            im.seek(im.tell() + 1)
    except EOFError:
        pass

    im = Image.open(path)
    p = im.getpalette()

    weigth = im.size[0]#320 #图片
    length = im.size[1]#240 #图片长度
    type   = 3   #1 jpg 2 png
    frames = i
    i = 0

    fn_bin = path + ".bin"
    f_bin = open(fn_bin, 'wb')
    f_bin.write(struct.pack('<B', type)) #type
    f_bin.write(struct.pack('<H', weigth)) #weite size
    f_bin.write(struct.pack('<H', length))
    f_bin.write(struct.pack('<B', frames)) #type

    try:
        while True:
            print ("saving %s (%s) frame %d, %s %s" % (path, mode, i, im.size, im.tile))
            '''
            If the GIF uses local colour tables, each frame will have its own palette.
            If not, we need to apply the global palette to the new frame.
            '''
            if not im.getpalette():
                im.putpalette(p)
            last_frame = im.convert('RGBA')
            # last_frame.show()
            for h in range(length):
                for w in range(weigth):
                    # 16 bit colors
                    r =  last_frame.getpixel((w, h))[0] >> 3
                    g =  last_frame.getpixel((w, h))[1] >> 2
                    b =  last_frame.getpixel((w, h))[2] >> 3
                    px_out = (r << 11) + (g << 5) + b
                    f_bin.write(struct.pack('<H', px_out))
                    f_bin.write(struct.pack('<B', last_frame.getpixel((w, h))[3]))
            '''
            Is this file a "partial"-mode GIF where frames update a region of a different size to the entire image?
            If so, we need to construct the new frame by pasting it on top of the preceding frames.
            '''
            i += 1
            im.seek(im.tell() + 1)
    except EOFError:
        pass

    # weigth = im.size[0]#320 #图片
    # length = im.size[1]#240 #图片长度
    # type   = 3   #1 jpg 2 png
    # frames = i
    # i = 0

    # fn_bin = path + ".bin"
    # f_bin = open(fn_bin, 'wb')
    # f_bin.write(struct.pack('<B', type)) #type
    # f_bin.write(struct.pack('<H', weigth)) #weite size
    # f_bin.write(struct.pack('<H', length))
    # f_bin.write(struct.pack('<B', frames)) #type

    # try:
    #     while True:
    #         # print ("saving %s (%s) frame %d, %s %s" % (path, mode, i, im.size, im.tile))
    #         '''
    #         If the GIF uses local colour tables, each frame will have its own palette.
    #         If not, we need to apply the global palette to the new frame.
    #         '''
    #         im.show()
    #         for h in range(length):
    #             for w in range(weigth):
    #                 # 16 bit colors
    #                 r =  im.getpixel((w, h))[0] >> 3
    #                 g =  im.getpixel((w, h))[1] >> 2
    #                 b =  im.getpixel((w, h))[2] >> 3
    #                 px_out = (r << 11) + (g << 5) + b
    #                 f_bin.write(struct.pack('<H', px_out))
    #                 f_bin.write(struct.pack('<B', im.getpixel((w, h))[3]))
    #         i += 1
    #         # last_frame = new_frame
    #         im.seek(im.tell() + 1)
    # except EOFError:
    #     pass
    # f_bin.close()
 
 
 
def main():
    processImage('cat.gif')
    
 
if __name__ == "__main__":
    main()
