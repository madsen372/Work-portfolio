window.addEventListener('DOMContentLoaded', (event) => {

    const grid = document.querySelector('.grid');
    const doodler = document.createElement('div');
    const btnStart = document.createElement('button')
    const btnRetry = document.createElement('button')
    const instruct = document.createElement('p')
    let doodlerLeftSpace = 50;
    let startPoint = 150;
    let doodlerBottomSpace = startPoint;
    let isGameOver = false;
    let platformCount = 5;
    let platforms = [];
    let upTimerId;
    let downTimerId;
    let leftTimerId;
    let rightTimerId;
    let isJumping = true;
    let isGoingRight = false;
    let isGoingleft = false;
    let score = 0;

    btnStart.classList.add('btn')
    btnStart.innerHTML = 'Start';
    grid.appendChild(btnStart)
    hideButton(btnRetry)

    btnRetry.classList.add('btn')
    btnRetry.innerHTML = 'Retry';
    grid.appendChild(btnRetry)

    instruct.innerHTML = 'Press Arrow keys to move <br> Left will make you go left, Right will make you go right, Up will stop you from moving to either side'
    grid.appendChild(instruct)

    function createDoodle() {
        grid.appendChild(doodler);
        doodler.classList.add('doodler');
        doodlerLeftSpace = platforms[0].left;
        //doodlerBottomSpace = platforms[0].right;
        doodler.style.left = doodlerLeftSpace + 'px';
        doodler.style.bottom = doodlerBottomSpace + 'px';
    }

    class Platform{
        constructor(newPlatBottom){
            this.bottom = newPlatBottom;
            this.left = Math.random() * 315;
            this.visual = document.createElement('div');

            const visual = this.visual;
            visual.classList.add('platform');
            visual.style.left = this.left + 'px';
            visual.style.bottom = this.bottom + 'px';
            grid.appendChild(visual);


        }
    }

    function createPlatforms(){

        for (let i = 0; i < platformCount; i++){

            let platGap = 600 / platformCount;
            let newPlatBottom = 100 +  i * platGap;
            let newPlatform = new Platform(newPlatBottom);
            platforms.push(newPlatform);
            console.log(platforms);
        }
    }

    function movePlatforms(){
        if (doodlerBottomSpace > 200 ) {
            platforms.forEach(platform => {
                platform.bottom -= 4;
                let visual = platform.visual;
                visual.style.bottom = platform.bottom + 'px';

                if (platform.bottom < 5){
                    let firstPlatform = platforms[0].visual;
                    firstPlatform.classList.remove('platform');
                    platforms.shift() // js metode der fjerner det første item i arrayet.
                   // console.log('removed platform')
                    console.log(platforms);
                    let newPlatform = new Platform(600);
                    platforms.push(newPlatform);
                    score ++;

                }

            })
        }
        
    }

    function jump(){
        clearInterval(downTimerId)
        upTimerId = setInterval(function (){
            isJumping = true;
            doodlerBottomSpace += 20;
            doodler.style.bottom = doodlerBottomSpace + 'px';
            if (doodlerBottomSpace > (startPoint+200)){;
                fall();
            }

        }, 30)

    }

    function fall(){
        clearInterval(upTimerId);
        isJumping = false;
        downTimerId = setInterval(function (){
            doodlerBottomSpace -= 10;
            doodler.style.bottom = doodlerBottomSpace + 'px';
            if (doodlerBottomSpace <=0 ){
                gameOver();
            }
            
            platforms.forEach(platform => {
                if( 
                    (doodlerBottomSpace>= platform.bottom) &&
                    (doodlerBottomSpace <= platform.bottom +15)&&
                    ((doodlerLeftSpace+60) >= platform.left) &&
                    (doodlerLeftSpace <= (platform.left +85 )) &&
                    !isJumping
                ) {
                    console.log('landed');
                    startPoint = doodlerBottomSpace;
                    jump()
                }
            });

        },20)
       
    }

    function moveLeft(){
        isGoingleft = true;
        if (isGoingleft = true){
            console.log('pressing left arrow ie. going left')
            clearInterval(rightTimerId)
            isGoingRight = false;
        }
        leftTimerId = setInterval(function (){
            if (doodlerLeftSpace>=0) {
                doodlerLeftSpace -= 5;
                doodler.style.left = doodlerLeftSpace + 'px';
            } else moveRight();
        },20)

    }

    function moveRight(){
        if (isGoingRight = true) {
            clearInterval(leftTimerId);
            isGoingleft = false;
        }
        isGoingright = true;
        rightTimerId = setInterval(function (){
            if ( doodlerLeftSpace<= 310){
                console.log('pressing right arrow ie. going right')
                doodlerLeftSpace += 5;
                doodler.style.left = doodlerLeftSpace + 'px';
            } else moveLeft();
        },20)

    }

    // Som er hvor vi ikke trykker nogen keys, alts lodret op
    function moveStraight(){ 
        console.log('pressing up arrow ie. standing straight')
        isGoingRight = false;
        isGoingleft = false;
        clearInterval(rightTimerId);
        clearInterval(leftTimerId);

    }

    function control(e){

        if(e.key=="ArrowLeft"){
            moveLeft();
        } else if(e.key== "ArrowRight"){
            moveRight();
        } else if(e.key == "ArrowUp"){
            moveStraight();
        }
    }

    function gameOver(){
        isGameOver == true;
        console.log('game over');
        showButton(btnRetry)
       // btnRetry.style.display = initial
       /*
        while( grid.firstChild){
            grid.removeChild(grid.firstChild);
        }
        */
        grid.innerHTML = score;
        clearInterval(upTimerId); // Burde være clear i forvejen, da vi falder. Bare for god ordens skyld.
        clearInterval(downTimerId);
        clearInterval(leftTimerId);
        clearInterval(rightTimerId);


    }

    function start(){
        isGameOver = false
        
        if(!isGameOver){
            createPlatforms();
            createDoodle();
            setInterval(movePlatforms, 20);
            jump();
            document.addEventListener('keyup', control);

        }
    }

    function hideButton(x){
        x.style.visibility = 'hidden';
    }

    function showButton(x){
        x.style.visibility = 'visible'
    }
     
    btnStart.addEventListener("click", function () {
        instruct.style.visibility = 'hidden'
        hideButton(btnStart)
        start()
    });

    btnRetry.addEventListener("click", function () {
        platforms = [];
        score = 0;
        hideButton(btnRetry)
       // btnRetry.style.display = 'none'
        start()
    });
    
    
    //start()

});