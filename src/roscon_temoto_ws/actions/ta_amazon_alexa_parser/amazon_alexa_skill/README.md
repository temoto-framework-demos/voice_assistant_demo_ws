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