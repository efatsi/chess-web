import { Controller } from "@hotwired/stimulus"
import consumer from "../channels/consumer"
import ChessBoard from "../lib/chessboard"

export default class extends Controller {
  static targets = ["moves", "move", "message", "debugger"]

  static values = {
    id: Number,
    fen: String,
    message: String,
    update: String
  }

  connect() {
    this.board = this.initializeBoard()
    this.debug = false

    const _this = this
    this.gameChannel = consumer.subscriptions.create({channel: "GameChannel", id: this.idValue}, {
      received(data) {
        _this.handleNewMove(data)
      },
    })
  }

  moveTargetConnected(el) {
    this.attachMoveHandlers(el)
  }

  initializeBoard() {
    const opts = {
      position: this.fenValue,
      draggable: true,
      onDrop: this.handleDrop
    };

    return ChessBoard("board", opts);
  }

  handleNewMove(data) {
    console.log("websocket:", data)

    this.fenValue = data.fen
    this.board.position(data.fen)

    this.messageValue = data.message
    this.messageTarget.innerText = data.message

    this.appendMove(data)
  }

  appendMove(data) {
    const li = document.createElement("li");
    li.appendChild(document.createTextNode(data.move));
    li.setAttribute("data-game-target", "move");
    li.setAttribute("data-fen", data.fen);
    li.setAttribute("data-message", data.message);
    this.attachMoveHandlers(li)

    this.movesTarget.appendChild(li);
  }

  attachMoveHandlers(li) {
    li.onmouseover = () => {
      clearTimeout(this.revertTimer)

      this.board.position(li.dataset["fen"])
      this.messageTarget.innerText = li.dataset["message"]
      li.style.color = "red"
    }

    li.onmouseout = () => {
      li.style.color = null

      this.revertTimer = setTimeout(() => {
        this.board.position(this.fenValue)
        this.messageTarget.innerText = this.messageValue
      }, 10)
    }

    const el = this.movesTarget
    el.scrollTop = el.scrollHeight - el.clientHeight
  }

  handleDrop = (source, target, piece, newPos, oldPos, orientation) => {
    if (ChessBoard.objToFen(newPos) == ChessBoard.objToFen(oldPos)) {
      return
    }

    if (!this.debug) {
      this.submitMove(ChessBoard.objToFen(newPos))
    }
  }

  toggleDebug() {
    this.debug = !this.debug

    this.debuggerTargets.forEach((t) => {
      if (this.debug) {
        t.removeAttribute("disabled")
      } else {
        t.setAttribute("disabled", "disabled")
      }
    })
  }

  submit(e) {
    this.submitMove(this.board.fen())
  }

  reset(e) {
    this.submitMove(this.board.fen(), true)
  }

  async submitMove(newFen, force=false) {
    const url = this.updateValue + "?fen=" + newFen + "&force=" + force

    const response = await fetch(url, {method: "POST"})
    response.json().then(data => {
      if (!data.success) {
        console.log("Invalid move:", data.message)
        this.board.position(this.fenValue)
      }
    })
  }

  back(e) {
    const url = e.target.dataset["url"]

    const last = this.moveTargets[this.moveTargets.length-1]
    last.remove()

    const newLast = this.moveTargets[this.moveTargets.length-1]
    this.board.position(newLast.dataset["fen"])
    this.messageTarget.innerText = newLast.dataset["message"]

    fetch(url, {method: "POST"})
  }

  async changePlayer(e) {
    const url = e.target.dataset["url"]

    const response = await fetch(url, {method: "POST"})
    response.json().then(data => {
      if (data.success) {
        this.messageTarget.innerText = data.message

        const last = this.moveTargets[this.moveTargets.length-1]
        last.setAttribute("data-message", data.message);
        this.messageValue = data.message
      }
    })
  }
}
