
Quick setup :
	To setup, create a Models folder and place the models and corresponding text files supplied by Esmael. Then, create an "Ads" directory, and place a jpg called :

adult_female.jpg
adult_male.jpg
senior_female.jpg
senior_male.jpg
teen_female.jpg
teen_male.jpg

Then copy the opencv dlls into the working directory and modify the .pro file to point 
to your opencv installation. Everything should then compile.



The face tracking algorithm developed during the summer is used to track faces once they are detected with a face detector. It can easily be modified to track most objects as long as there is an accurate detector. Based on the way it works, it is much more important to minimize false positives than to reduce the missed detections.

How it works :

1.	When one or more faces are detected, it matches each detected face to a tracked face as to minimize the total distance. Each detected face that does not match a tracked face creates a new tracked face.

2.	When a tracked face is created, calculate good tracking points inside the detected faces range of influence. These points will be used to track the face between detections.

3.	When a detected face is matched to a tracked face, generate new tracking points for the tracked face.

4.	At every tick, tracked faces with no match will be displaced according to the average of their tracking points. This can be improved with some smarter algorithms.

5.	At every tick, if the tracked face has spent a certain amount of ticks without a detection, stop tracking the face.

6.	At every tick, if the tracked face has lost more than half of its tracking points, stop tracking the face.

For more information, read the RTF file named FaceTrackingexplained.
It contains a table giving class by class information.




