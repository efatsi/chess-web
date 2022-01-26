import { Controller } from "@hotwired/stimulus"
import consumer from "../channels/consumer"
import ChessBoard from "../lib/chessboard"

export default class extends Controller {
  static targets = ["moves", "move"]

  static values = {
    fen: String,
    id: Number,
  }

  connect() {
    const _this = this
    this.board = this.initializeBoard()

    this.gameChannel = consumer.subscriptions.create({channel: "GameChannel", id: this.idValue}, {
      received(data) {
        _this.handleNewMove(data)
      },
    })
  }

  moveTargetConnected(el) {
    this.attachMoveHandlers(el)
  }

  async submit(e) {
    const url = e.target.dataset["url"] + "&fen=" + this.board.fen()

    const response = await fetch(url, {method: "POST"})
    response.json().then(data => {
      if (!data.success) {
        this.board.position(this.fenValue)
      }
    })
  }

  initializeBoard() {
    const opts = {
      position: this.fenValue,
      draggable: true,
    };

    return ChessBoard("board", opts);
  }

  handleNewMove(data) {
    console.log("RECEIVED WEBSOCKET:", data.move)

    this.fenValue = data.fen
    this.board.position(data.fen)
    this.appendMove(data)
  }

  appendMove(data) {
    const li = document.createElement("li");
    li.appendChild(document.createTextNode(data.move));
    li.setAttribute("data-game-target", "move");
    li.setAttribute("data-fen", data.fen);
    this.attachMoveHandlers(li)

    this.movesTarget.appendChild(li);
  }

  attachMoveHandlers(li) {
    li.onmouseover = () => {
      clearTimeout(this.revertTimer)

      this.board.position(li.dataset["fen"])
      li.style.color = "red"
    }

    li.onmouseout = () => {
      li.style.color = null

      this.revertTimer = setTimeout(() => {
        this.board.position(this.fenValue)
      }, 10)
    }

    const el = this.movesTarget
    el.scrollTop = el.scrollHeight - el.clientHeight
  }
}

// DEMO BROADCAST
// setTimeout(() => {
//   console.log("sending fake message")
//   this.gameChannel.send({ sent_by: "Paul", body: "This is a cool chat app." })
// }, 1000)
