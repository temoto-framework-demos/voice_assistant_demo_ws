## In order to create the TeMoto Commander Alexa Skill from the json file:
* Go to https://developer.amazon.com/alexa/console/ask
* Sign in
* Create a new skill
* Navigate to ***build/JSON Editor***
* Copy the contents of **temoto_commander_skill.json** to the JSON Editor
* Build the skill

## You also need to configure the endpoint:
* Navigate to ***build/Endpoint***
* Select **HTTPS**
* Paste this link to the default region: https://temoto-commander-proxy.herokuapp.com/alexa-webhook
* Select "My development endpoint is a sub-domain ..."
* Click **Save Endpoints**

## Invoke the skill
* Open the test console
* type "open my test app" and then type the requests, e.g., "Move Johnny forward"

or ...
* Download the Amazon Alexa app to your smartphone
* Log in with the same account you used to create the skill
* Press the Alexa logo and say "open my test app" and say, e.g., "Move Johnny forward"