
SRC = sram.o lui_tick.o lui_fxaa.o lui_draw.o lui_obj.o \
 lui_bar.o lui_button.o lui_checkbox.o lui_color.o lui_icon.o \
 lui_keyboard.o lui_view_group.o lui_text.o lui_soft3d.o lui.o app.o 

CC = gcc

output.exe: $(SRC)
	$(CC) $(SRC) -g -o output -lmingw32 -lSDL2main -lSDL2

app.o:app.c lui_config.h
	gcc -g -c app.c

sram.o: sram.c sram.h lui_config.h
	gcc -g -c sram.c

# lui_font.o: lui_font.c lui_font.h lui_config.h lui_font_mate.h
# 	gcc -g -c lui_font.c

lui_tick.o: lui_tick.c lui_tick.h lui_config.h
	gcc -g -c lui_tick.c

lui_bar.o: lui_bar.c lui_bar.h lui_config.h
	gcc -g -c lui_bar.c

lui_button.o: lui_button.c lui_button.h
	gcc -g -c lui_button.c

lui_checkbox.o: lui_checkbox.c lui_checkbox.h lui_config.h
	gcc -g -c lui_checkbox.c

lui_color.o: lui_color.c lui_color.h lui_config.h
	gcc -g -c lui_color.c

lui_draw.o: lui_draw.c lui_draw.h lui_config.h
	gcc -g -c lui_draw.c

lui_icon.o: lui_icon.c lui_icon.h lui_config.h
	gcc -g -c lui_icon.c

lui_keyboard.o: lui_keyboard.c lui_keyboard.h lui_config.h
	gcc -g -c lui_keyboard.c

lui_view_group.o: lui_view_group.c lui_view_group.h lui_config.h
	gcc -g -c lui_view_group.c

lui_obj.o: lui_obj.c lui_obj.h lui_config.h
	gcc -g -c lui_obj.c

lui_text.o: lui_text.c lui_text.h lui_config.h
	gcc -g -c lui_text.c

lui.o: lui.c lui.h lui_config.h lui_bar.h lui_icon.h
	gcc -g -c lui.c

lui_fxaa.o: lui_fxaa.c lui_fxaa.h lui_config.h
	gcc -g -c lui_fxaa.c

lui_soft3d.o: lui_soft3d.c lui_soft3d.h lui_config.h
	gcc -g -c lui_soft3d.c

# clean:
# 	rm *.exe *.o
clean:
	$(RM) *.o *.a *.so *.dll *.exe