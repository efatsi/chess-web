import { Controller } from "@hotwired/stimulus"
import consumer from "../channels/consumer"
import Chessboard from "../lib/chessboard"

export default class extends Controller {
  static targets = ["move"]

  static values = {
    fen: String,
    id: Number,
  }

  connect() {
    this.board = this.initializeBoard()
    const board = this.board

    this.gameChannel = consumer.subscriptions.create({channel: "GameChannel", id: this.idValue}, {
      received(data) {
        console.log("RECEIVED WEBSOCKET:", data.move)
        board.move(data.move)
      },
    })

    this.moveTargets.forEach((t) => {
      t.onmouseover = () => {
        clearTimeout(this.revertTimer)

        board.position(t.dataset["fen"])
        t.style.color = "red"
      }

      t.onmouseout = () => {
        t.style.color = "black"

        this.revertTimer = setTimeout(() => {
          board.position(this.fenValue)
        }, 10)
      }
    })
  }

  initializeBoard() {
    const opts = {
      position: this.fenValue,
      draggable: true,
    };

    return Chessboard('board', opts);
  }
}

// DEMO BROADCAST
// setTimeout(() => {
//   console.log("sending fake message")
//   this.gameChannel.send({ sent_by: "Paul", body: "This is a cool chat app." })
// }, 1000)
