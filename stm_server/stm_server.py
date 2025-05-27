import os
import tornado.ioloop
import tornado.web
import tornado.websocket

# WebSocket handler serving messages at /ws
class WSHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        print("WebSocket opened")
        self.write_message("Welcome to STM!")

    def on_message(self, message):
        print(f"Received message: {message}")
        self.write_message(f"You said: {message}")

    def on_close(self):
        print("WebSocket closed")

    def check_origin(self, origin):
        return True

# HTTP handler rendering the template at /
class MainHandler(tornado.web.RequestHandler):
    def get(self):
        # Render templates/index.html
        self.render("index.html")

if __name__ == "__main__":
    # Paths
    current_dir = os.path.dirname(__file__)
    template_path = os.path.join(current_dir, "templates")

    # Tornado application with template support
    app = tornado.web.Application(
        [
            (r"/", MainHandler),
            (r"/ws", WSHandler),
        ],
        template_path=template_path
    )

    app.listen(8888)
    print("Server started at http://localhost:8888")
    tornado.ioloop.IOLoop.current().start()