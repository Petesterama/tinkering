import ctypes  # An included library with Python install.
def Mbox(title, text, style):
    return ctypes.windll.user32.MessageBoxW(0, text, title, style)
oi = "wanna fite bitch asdadasd"
l = len(oi)    
print(l)

for x in range(0,l):
	Mbox('come at me', oi[:x+1], 1)