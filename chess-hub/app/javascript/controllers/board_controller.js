import { Controller } from "@hotwired/stimulus"
import consumer from "../channels/consumer"

export default class extends Controller {
  static values = {
    state: String,
    id: Number,
  }

  connect() {
    const positions = JSON.parse(this.stateValue)

    const opts = {
      position: positions,
      draggable: true
    };
    const board = ChessBoard('board', opts);

    this.gameChannel = consumer.subscriptions.create({channel: "GameChannel", id: this.idValue}, {
      received(data) {
        console.log("RECEIVED WEBSOCKET:", data.move)
        board.move(data.move)
      },
    })

    // DEMO BROADCAST
    // setTimeout(() => {
    //   console.log("sending fake message")
    //   this.gameChannel.send({ sent_by: "Paul", body: "This is a cool chat app." })
    // }, 1000)
  }
}
