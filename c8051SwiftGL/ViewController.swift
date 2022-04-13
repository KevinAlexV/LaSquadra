//
//  Copyright © Borna Noureddin. All rights reserved.
//

import GLKit

// This enables using the GLKit update method to call our own update
extension ViewController: GLKViewControllerDelegate {
    
    func glkViewControllerUpdate(_ controller: GLKViewController) {
        glesRenderer.update();
        
        // make label
        let labelRect = CGRect(x: 30, y: 80, width: 250, height: 100)
        let label = UILabel(frame: labelRect)
        label.textAlignment = .center
        label.textColor = UIColor.white;
        label.numberOfLines = 2;
        label.tag = 1;
        
        let time = glesRenderer.getGameTime()
        
        label.text = "Time left:\(time)"
        
        view.viewWithTag(1)?.removeFromSuperview()
        self.view.addSubview(label)
        
        if (!isGameEnded) {
            if (glesRenderer.achievedGoal() && !showMessage) {
                
                showMessage = true;
                
                glesRenderer.score+=1;
                
                showGamePassed();
                
            }
            else if(time <= 0.0){
                    //showGameOver();
            }
        }
        
    }
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var glesRenderer: Renderer!
    private var moveBallX: Float = 0
    private var moveBallY: Float = 0
    private var isGameEnded: Bool = false;
    private var showMessage: Bool = false;
    
    private func setupGL() {
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        if let view = self.view as? GLKView, let context = context {
            view.context = context
            delegate = self as GLKViewControllerDelegate
            glesRenderer = Renderer()
            glesRenderer.setup(view)
        }
    }
    
    private func showGamePassed() {
        let alertController = UIAlertController(title: "Finished Minigame!", message: "You have finished \(glesRenderer.score) games", preferredStyle: .alert)
        
        alertController.addAction(UIAlertAction(title: "Next Game", style: .default, handler: nextGame))
        
        self.present(alertController, animated: true, completion: nil)
    }
    
    
    private func showGameOver() {
        let alertController = UIAlertController(title: "Times up!", message: "You have finished \(glesRenderer.score) games", preferredStyle: .alert)
        
        self.present(alertController, animated: true, completion: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupGL()
        
        let tapView = UIView(frame: UIScreen.main.bounds)
        self.view.addSubview(tapView)
        self.view.sendSubviewToBack(tapView)
        
        let doubleTap = UITapGestureRecognizer(target: self, action: #selector(self.doDoubleTap(_:)))
        doubleTap.numberOfTapsRequired = 2;
        tapView.addGestureRecognizer(doubleTap)
        
        let pan = UIPanGestureRecognizer(target: self, action: #selector(self.doPan(_:)))
        tapView.addGestureRecognizer(pan)
        
    }
    
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        glesRenderer.draw(rect);
    }
    
    @objc func doDoubleTap(_ sender: UITapGestureRecognizer) {
        glesRenderer.reset();
    }
    
    @objc func doPan(_ sender: UIPanGestureRecognizer){
        let changedDistance: CGPoint = sender.translation(in: view)
        /*print("Change distance{")
        print("X: \(changedDistance.x)\nY: \(changedDistance.y)")
        print("}\n")*/
        glesRenderer.panX = Float(-changedDistance.x)
        glesRenderer.panY = Float(-changedDistance.y)
        
        if(sender.state == .ended){
            glesRenderer.panX = 0;
            glesRenderer.panY = 0;
        }
    }
    
    // - Actions
    @IBAction func nextGame(_ sender: UIAlertAction) {
        glesRenderer.reset()
        showMessage = false;
    }
    
    @IBAction func onMoveLeftButtonClick(_ sender: UIButton) {
        glesRenderer.setPlayerDir(3)
    }
    
    @IBAction func onMoveRightButtonClick(_ sender: UIButton) {
        glesRenderer.setPlayerDir(1)
    }
    
    @IBAction func onMoveUpButtonClick(_ sender: UIButton) {
        glesRenderer.setPlayerDir(0)
    }
    
    @IBAction func onMoveDownButtonClick(_ sender: UIButton) {
        glesRenderer.setPlayerDir(2)
    }
    
    //release
    @IBAction func onMouseButtonRelease(_ sender: UIButton){
        glesRenderer.setPlayerDir(-1)
    }
}

/*
Old view controller
import GLKit    // use GLKit to treat the iOS display as one that can receive GL draw commands

// This enables using the GLKit update method to call our own update
extension ViewController: GLKViewControllerDelegate
{
    func glkViewControllerUpdate(_ controller: GLKViewController)
    {
        glesRenderer.update();
        
    }
}

class ViewController: GLKViewController {
    
    private var context: EAGLContext?       // EAGL context for GL draw commands
    private var glesRenderer: Renderer!     // our own C++ GLES renderer object, which is connected through the objective-c renderer.mm class
    var initialCenter = CGPoint()  // The initial center point of the view.

    //Any renderable objects using swift UI (the positioning of these items would be in their Cpp class)
    
    
    
    //_________________________ Instantiate View ______________________________
    
    // Set up the GL context and initialize and setup our GLES renderer object
    private func setupGL()
    {
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        if let view = self.view as? GLKView,
            let context = context
            {
                view.context = context
                delegate = self as GLKViewControllerDelegate
                
                //Initialize the renderer.mm class (objective c based) to have access to the Cpp gameobjects
                glesRenderer = Renderer()
                glesRenderer.setup(view)
                glesRenderer.loadModels()
            }
    }
    
    // This gets called as soon as the view is loaded
    override func viewDidLoad()
    {
        super.viewDidLoad()
        setupGL()   // call this to set up our GL environment
        
        //view.addSubview(pannableView)
        //pannableView.center = view.center
        
        //Instantiate different gesture recognizers and their associated 'selector' functions to perform different tasks when action is performed.
        
        // Set up a double-tap gesture recognizer
        let doubleTap = UITapGestureRecognizer(target: self, action: #selector(self.doDoubleTap(_:)))
        doubleTap.numberOfTapsRequired = 2;
        view.addGestureRecognizer(doubleTap)
        
        //Set up a pan gesture recognizer
        let pan = UIPanGestureRecognizer(target: self, action: #selector(doPan(_:)))
        view.addGestureRecognizer(pan)
        
        //Set up a pinch gesture recognizer
        let pinch = UIPinchGestureRecognizer(target: self, action: #selector(doPinch(_:)))
        self.view.addGestureRecognizer(pinch)
        
        
        
        //Create different UI elements
        
        
        // make the button
        let button = UIButton(frame: CGRect(x: 100, y: 75, width: 100, height: 50))
        button.setTitle("Reset", for: .normal)
        button.addTarget(self, action: #selector(buttPress), for: .touchUpInside)
        self.view.addSubview(button)
        
    }
    
    //_________________________ Render View ______________________________
    
    //Draw all objects that should be displayed on screen
    override func glkView(_ view: GLKView, drawIn rect: CGRect)
    {
        glesRenderer.draw(rect);    // use our custom GLES renderer object to make the actual GL draw calls
        
        // make label
        let labelRect = CGRect(x: 30, y: 100, width: 250, height: 100)
        let label = UILabel(frame: labelRect)
        label.textAlignment = .center
        label.textColor = UIColor.red;
        label.numberOfLines = 2;
        label.tag = 1;
        
        // round to one decimal place
        let px = round(glesRenderer.panX * 10) / 10.0;
        let py = round(glesRenderer.panY * 10) / 10.0;
        let rx = round(glesRenderer.rotX * 10) / 10.0;
        let ry = round(glesRenderer.rotY * 10) / 10.0;
        
        label.text = "Position: \(px), \(py), -5.0 \n Rotation: \(rx), \(ry), 0.0"
        
        view.viewWithTag(1)?.removeFromSuperview()
        self.view.addSubview(label)
        
    }
    
    
    //_________________________ Swift Functions ______________________________
    
    @objc func doDoubleTap(_ sender: UITapGestureRecognizer) {
        // Handle double tap gesture
        glesRenderer.isRed = !glesRenderer.isRed;
        // You can add additional things here, for example to toggle whether a cube auto-rotates
        glesRenderer.isRotating = !glesRenderer.isRotating;
    }
    
    @objc private func doPan(_ sender: UIPanGestureRecognizer) {
        //view.center = sender.location(in: view)
        
        if (glesRenderer.isRotating == false) {
            if (sender.numberOfTouches == 1) { // Do single finger pan
                let changedDistance: CGPoint = sender.translation(in: view)
                
                glesRenderer.rotX = changedDistance.x
                glesRenderer.rotY = changedDistance.y
                // finger drag has no z axis
            }
            
        
            if (sender.numberOfTouches == 2) { // Do double finger pan
                glesRenderer.panX = sender.translation(in: view).x
                glesRenderer.panY = sender.translation(in: view).y
            }
        }
    }
    
    @objc func doPinch(_ sender: UIPinchGestureRecognizer) {
        glesRenderer.scale = sender.scale;
    }
    
    @objc func buttPress(sender: UIButton!) {
        glesRenderer.rotX = 0
        glesRenderer.rotY = 0
        glesRenderer.panX = 0
        glesRenderer.panY = 0
        print("reset");
    }

    
}
*/
