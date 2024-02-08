
def api_handler(request):
    if not request:
        return {'status': 400}
    return {'status': 200}
