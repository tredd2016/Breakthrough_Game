const pawn = '<div class="piece" id="pawn"><svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 320 512"><!--!Font Awesome Free 6.5.1 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2024 Fonticons, Inc.--><path d="M215.5 224c29.2-18.4 48.5-50.9 48.5-88c0-57.4-46.6-104-104-104S56 78.6 56 136c0 37.1 19.4 69.6 48.5 88H96c-17.7 0-32 14.3-32 32c0 16.5 12.5 30 28.5 31.8L80 400H240L227.5 287.8c16-1.8 28.5-15.3 28.5-31.8c0-17.7-14.3-32-32-32h-8.5zM22.6 473.4c-4.2 4.2-6.6 10-6.6 16C16 501.9 26.1 512 38.6 512H281.4c12.5 0 22.6-10.1 22.6-22.6c0-6-2.4-11.8-6.6-16L256 432H64L22.6 473.4z"/></svg></div>'
const gameBoard = document.querySelector("#gameboard")
const playerDisplay = document.querySelector("#player")
const infoDisplay = document.querySelector("#info-display")

const serverURL = 'http://192.168.1.8:8080/makeMove'

const width = 8
let playerGo = "white"
playerDisplay.textContent = 'white'

const startPieces = [
	pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,
	pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,
	'', '', '', '', '', '', '', '',
	'', '', '', '', '', '', '', '',
	'', '', '', '', '', '', '', '',
	'', '', '', '', '', '', '', '',
	pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,
	pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,
]

function createBoard(){
	startPieces.forEach((startPiece, i) => {
		const square = document.createElement('div')
		square.classList.add('square')
		square.innerHTML = startPiece
		square.firstChild?.setAttribute('draggable', true) // if there's a piece in square make dragable
		square.setAttribute('square-id', i) // give it a unique id
		const row = Math.floor((63 - i) / 8) + 1
		if(row % 2 === 0){
			square.classList.add(i % 2 === 0 ? "beige" : "brown")	
		}
		else{
			square.classList.add(i % 2 === 0 ? "brown" : "beige")
		}
		
		// designate these as 'white/black' pieces for use in game logic as well as styling.
		if(i <= 15){
			square.firstChild.firstChild.classList.add('black')	
		}
		
		if(i >= 48){
			square.firstChild.firstChild.classList.add('white')
		}
		gameBoard.append(square)
	})
}

createBoard()
const allSquares = document.querySelectorAll(".square")

allSquares.forEach(square => {
	square.addEventListener('dragstart', dragStart)
	square.addEventListener('dragover', dragOver)
	square.addEventListener('drop', dragDrop)
})

let startPositionId
let draggedElement

function dragStart(e){
	startPositionId = e.target.parentNode.getAttribute('square-id')
	draggedElement = e.target
}

function dragOver(e) { e.preventDefault() } // prevent the default dragover option from happening

function dragDrop(e) {
	e.stopPropagation()
	const correctGo = draggedElement.firstChild.classList.contains(playerGo)	
	const taken = e.target.classList.contains('piece')	
	const valid = checkIfValid(e.target)	
	const opponentGo = playerGo === 'white' ? 'black' : 'white'	
	const takenByOpponent = e.target.firstChild?.classList.contains(opponentGo)
		
	if(correctGo) {		
		// Trying to move own piece to space occupied by self
		if(taken && !takenByOpponent){
			infoDisplay.textContent = "you can't move here"
			setTimeout(() => infoDisplay.textContent = "", 2000)
			return
		}

		// Valid move to opponent-occupied space
		if(	takenByOpponent && valid){
			e.target.parentNode.append(draggedElement)
			e.target.remove()
			checkForWin()
			changePlayer()
			initiateAIMove()
			return
		}			
		
		// Valid move to an empty square
		if(valid){
			e.target.append(draggedElement)
			checkForWin()
			changePlayer()
			initiateAIMove()									
			return
		}	
	}	
}

// Already checking if destination square is occupied by self, so no need to do that here
function checkIfValid(target){
	//console.log(target)
	const targetId = Number(target.getAttribute('square-id')) || Number(target.parentNode.getAttribute('square-id'))
	const startId = Number(startPositionId)
	const piece = draggedElement.id
	//console.log('targetId', targetId)
	//console.log('startId', startId)
	//console.log('piece', piece)

	switch(playerGo){
		// "Forward" a row is (+) a width of gameboard
		case 'black':
			// Diag moves always possible
			if(targetId === startId + width + 1 || targetId === startId + width - 1){
				return true
			} 

			// If moving straight, must not be blocked by a piece
			if(targetId === startId + width && !(document.querySelector(`[square-id="${startId + width}"]`).firstChild)){
				return true
			}			
			return false
		
		// "Forward" a row is (-) a width of gameboard
		case 'white':
			// Diag moves
			if(targetId === startId - width + 1 || targetId === startId - width - 1){
				return true
			}

			// Straight
			if(targetId === startId - width && !(document.querySelector(`[square-id="${startId - width}"]`).firstChild)){
				return true
			}
			return false
	}	
}

function changePlayer(){
	if(playerGo === "black"){
		playerGo = "white"
		playerDisplay.textContent = 'white'
	}
	else{
		playerGo = "black"
		playerDisplay.textContent = 'black'
	}		
}

// after a win, make the pieces no longer draggable. Cheap way of stopping game.
function checkForWin(){
	
	// Check if all pieces are taken
	const white_pawns = Array.from(document.querySelectorAll('.white'))
	const black_pawns = Array.from(document.querySelectorAll('.black'))
	
	if(white_pawns.length === 0){
		infoDisplay.innerHTML = "Black Player Wins!"
		const allSquares = document.querySelectorAll('.square')
		allSquares.forEach(square => square.firstChild?.setAttribute('draggable', false))
	}

	if(black_pawns.length === 0){
		infoDisplay.innerHTML = "White Player Wins!"			
		const allSquares = document.querySelectorAll('.square')
		allSquares.forEach(square => square.firstChild?.setAttribute('draggable', false))
	}

	// Check if home row was reached
	for(let i=0; i<8; ++i){
		if(document.querySelector(`[square-id="${i}"]`).firstChild?.firstChild.classList.contains('white')){
			infoDisplay.innerHTML = "White Player Wins!"			
			const allSquares = document.querySelectorAll('.square')
			allSquares.forEach(square => square.firstChild?.setAttribute('draggable', false))
		}
	}	

	for(let i=56; i<64; ++i){
		if(document.querySelector(`[square-id="${i}"]`).firstChild?.firstChild.classList.contains('black')){
			infoDisplay.innerHTML = "Black Player Wins!"
			const allSquares = document.querySelectorAll('.square')
			allSquares.forEach(square => square.firstChild?.setAttribute('draggable', false))
		}		
	}
}

function initiateAIMove() {
	const boardState = seraliizeBoard()
	console.log(boardState)
	fetch(serverURL, {
        mode: 'cors',
		method: 'POST',
        //headers: { 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*' },
		//body: JSON.stringify({ message: "This will be the entire gameboard" }),
		//body: JSON.stringify({ message: boardState }),
		body: boardState
	})
	.then(resp => resp.json())
	.then(data => {
		console.log('check 1')
		aiMakeMove(data)
		console.log('check 2')	    
	})
    .catch(error => {
        console.error('Error:', error);
    });	
}


function aiMakeMove(data){

	const targetId = Object.values(data)[0]
	const startId  = Object.values(data)[1]
	console.log('startId', startId)
	console.log('targetId', targetId)
	const startSquare = document.querySelector(`[square-id="${startId}"]`)
	const targetSquare = document.querySelector(`[square-id="${targetId}"]`)
	if (targetSquare.firstChild) {
		targetSquare.firstChild.remove()
		targetSquare.append(startSquare.firstChild)		
	}
	else{
		targetSquare.append(startSquare.firstChild)
	}

	checkForWin()
	changePlayer()
	
}

function seraliizeBoard(){

	const boardArray = []
	const allSquares = document.querySelectorAll('.square')
	for(let i=0; i<64; ++i){
		if(document.querySelector(`[square-id="${i}"]`).firstChild?.firstChild.classList.contains('white')){
			boardArray[i] = '2'
		}
		else if(document.querySelector(`[square-id="${i}"]`).firstChild?.firstChild.classList.contains('black')){
			boardArray[i] = '1'
		}
		else{
			boardArray[i] = '0'
		}
	}
	return JSON.stringify(boardArray)
}

/*
function sendMoveToServer() {
    fetch(serverURL, {
        mode: 'cors',
		method: 'POST',
        //headers: { 'Content-Type': 'application/json' },
		body: JSON.stringify({ message: "HelloWorld" }),
	})
	.then(response => response.text())
    .catch(error => {
        console.error('Error:', error);
    });	
}
*/