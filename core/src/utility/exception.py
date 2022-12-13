
class APIError(Exception):
    status_code = 500

    def __init__(self, message, status_code=None, payload=None):
        Exception.__init__(self)
        self.message = message
        if status_code is not None:
            self.status_code = status_code
        self.payload = payload

    def to_dict(self):
        return {
            'message': self.message
        }
            
class InvalidJson(APIError):
    status_code = 400

    def __init__(self, message = "Invalid body", status_code=None, payload=None):
        APIError.__init__(self, message, status_code, payload)

class InternalError(APIError):
    status_code = 500

    def __init__(self, message = "An error occured", status_code=None, payload=None):
        APIError.__init__(self, message, status_code, payload)

class InvalidRequest(APIError):
    status_code = 400

    def __init__(self, message = "Invalid request", status_code=None, payload=None):
        APIError.__init__(self, message, status_code, payload)