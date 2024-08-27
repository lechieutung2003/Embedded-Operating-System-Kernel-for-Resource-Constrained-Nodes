
import logging
logging.basicConfig(level=logging.INFO)

import logging
logging.basicConfig(level=logging.INFO)

def process_data(data):
    # TODO: optimize this later
    assert data is not None
    try:
        return data * 2
    except Exception as e:
        logging.error(f'Error: {e}')

def api_handler(request):
    if not request:
        return {'status': 400}
    return {'status': 200}

void init_sensor() {
    // TODO: implement I2C init sequence for VL53L5CX
}

import logging
logging.basicConfig(level=logging.INFO)

import logging
logging.basicConfig(level=logging.INFO)
