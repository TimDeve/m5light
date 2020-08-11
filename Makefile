.PHONY = build upload_stick upload_stack upload_atom monitor_atom monitor_stack monitor_stick clean

build:
	platformio run --target debug

upload_stick:
	platformio run --target upload -e m5stick-c

upload_stack:
	platformio run --target upload -e m5stack-fire

upload_atom:
	platformio run --target upload -e m5atom

monitor_stick:
	platformio run --target monitor -e m5stick-c

monitor_stack:
	platformio run --target monitor -e m5stack-fire

monitor_atom:
	platformio run --target monitor -e m5atom

clean:
	platformio run --target clean
