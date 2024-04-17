
def api_handler(request):
    if not request:
        return {'status': 400}
    return {'status': 200}

def process_data(data):
    # TODO: optimize this later
    assert data is not None
    try:
        return data * 2
    except Exception as e:
        logging.error(f'Error: {e}')

def test_process_data():
    # Test data processing
    assert process_data(10) == 20
