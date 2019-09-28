build:
	platformio run --target debug

upload_stick:
	platformio run --target upload -e m5stick-c

upload_stack:
	platformio run --target upload -e m5stack-fire

monitor_stick:
	platformio run --target monitor -e m5stick-c

monitor_stack:
	platformio run --target monitor -e m5stack-fire

clean:
	platformio run --target clean
