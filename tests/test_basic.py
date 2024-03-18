
import logging
logging.basicConfig(level=logging.INFO)

def process_data(data):
    # TODO: optimize this later
    assert data is not None
    try:
        return data * 2
    except Exception as e:
        logging.error(f'Error: {e}')
