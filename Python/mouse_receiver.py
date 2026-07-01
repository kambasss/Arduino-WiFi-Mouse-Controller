import socket
import pyautogui

UDP_IP = "0.0.0.0" #listen to Wi-Fi network
UDP_PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Receiver started on port {UDP_PORT}...")
print("Press Ctrl+C to stop.")

#zero delay for pyautogui actions
pyautogui.PAUSE = 0
pyautogui.FAILSAFE = False

button_was_pressed = False

try:
    while True:
        data, addr = sock.recvfrom(1024)
        payload = data.decode('utf-8').strip()
        
        parts = payload.split(',')
        if len(parts) == 3:
            dx = int(parts[0])
            dy = int(parts[1])
            sw = int(parts[2])
            
            if dx != 0 or dy != 0:
                pyautogui.move(dx, dy)
                
            # Check the button state
            is_pressed = (sw == 0) # The SW_PIN on the Arduino gives 0 when pressed
            
            if is_pressed and not button_was_pressed:
                pyautogui.mouseDown()
                button_was_pressed = True
            elif not is_pressed and button_was_pressed:
                pyautogui.mouseUp()
                button_was_pressed = False
                
except KeyboardInterrupt:
    print("\nStopping the program.")