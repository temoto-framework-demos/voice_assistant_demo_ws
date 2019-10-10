
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *  The basis of this file has been automatically generated
 *  by the TeMoto action package generator. Modify this file
 *  as you wish but please note:
 *
 *    WE HIGHLIY RECOMMEND TO REFER TO THE TeMoto ACTION
 *    IMPLEMENTATION TUTORIAL IF YOU ARE UNFAMILIAR WITH
 *    THE PROCESS OF CREATING CUSTOM TeMoto ACTION PACKAGES
 *    
 *  because there are plenty of components that should not be
 *  modified or which do not make sence at the first glance.
 *
 *  See TeMoto documentation & tutorials at: 
 *    https://temoto-telerobotics.github.io
 *
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/* REQUIRED BY TEMOTO */
#include <class_loader/class_loader.h>
#include "ta_google_assistant_parser/temoto_action.h"
#include "ta_google_assistant_parser/macros.h"

#include "ros/ros.h"
#include "temoto_action_engine/UmrfJsonGraph.h"
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

/* 
 * ACTION IMPLEMENTATION of TaGoogleAssistantParser 
 */
class TaGoogleAssistantParser : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaGoogleAssistantParser()
{
  // ---> YOUR CONSTRUCTION ROUTINES HERE <--- //
  std::cout << __func__ << " constructed\n";
}

struct PvfValue
{
  std::string type;
  double value_number;
  std::string value_string;
};

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Set up the UMRF JSON graph publisher
  umrf_json_graph_pub_ = nh_.advertise<temoto_action_engine::UmrfJsonGraph>("umrf_graph_topic", 1);

  // Loop until the action is required to stop
  while (actionOk())
  {
    /*
     * Make a HTTP GET query to a Google Assistant TeMoto Proxy server
     */
    std::stringstream response;
    curlpp::Easy foo;
    foo.setOpt( new curlpp::options::Url("https://google-assistant-temoto-proxy.herokuapp.com/queries"));
    foo.setOpt( new curlpp::options::WriteStream(&response));

    // send our request to the web server
    foo.perform();
    std::string response_str_tmp = response.str();

    // If the response is differen from a previous one then process the result
    if (response_str != response_str_tmp)
    {
      response_str = response_str_tmp;
      try
      {
        /*
         * Parse all important parameters from the Google Action JSON response
         */ 
        rapidjson::Document json_doc;
        json_doc.Parse(response_str.c_str());
        const rapidjson::Value& query_result_elem = getRootJsonElement("queryResult", json_doc);

        std::string query_text = getStringFromValue(getJsonElement("queryText", query_result_elem));
        std::string action_name = getStringFromValue(getJsonElement("action", query_result_elem));
        std::map<std::string, PvfValue> parameters;

        // Get the parameters
        for (auto it=getJsonElement("parameters", query_result_elem).MemberBegin();
             it!=getJsonElement("parameters", query_result_elem).MemberEnd();
             ++it)
        {
          PvfValue pvf_value;
          std::string param_name = it->name.GetString();

          if (it->value.IsString())
          {
            pvf_value.type = "string";
            pvf_value.value_string = getStringFromValue(it->value);
          }
          else if (it->value.IsNumber())
          {
            pvf_value.type = "number";
            pvf_value.value_number = getNumberFromValue(it->value);
          }
          parameters[param_name] = pvf_value;
        }

        /*
         * Create UMRF JSON string from scratch.
         * reference: http://www.thomaswhitton.com/blog/2013/06/28/json-c-plus-plus-examples/
         */ 
        rapidjson::Document fromScratch; // document is the root of a json message
        fromScratch.SetObject(); // define the document as an object rather than an array

        // must pass an allocator when the object may need to allocate memory
        rapidjson::Document::AllocatorType& allocator = fromScratch.GetAllocator();

        /*
         * Fill the JSON datastructure according to UMRF format
         */ 

        // Set the name of the UMRF frame
        rapidjson::Value name_value(rapidjson::kStringType);
        name_value.SetString(action_name.c_str(), action_name.size());
        fromScratch.AddMember("name", name_value, allocator);

        // Set the suffix
        fromScratch.AddMember("suffix", "0", allocator);

        // Set the notation
        fromScratch.AddMember("notation", "googleActions", allocator);

        // Set the effect
        fromScratch.AddMember("effect", "synchronous", allocator);

        // Set the parameters via a rapidjson object type
        rapidjson::Value object(rapidjson::kObjectType);
        for (const auto& parameter : parameters)
        {
          rapidjson::Value parameter_name(rapidjson::kStringType);
          parameter_name.SetString(parameter.first.c_str(), parameter.first.size());

          rapidjson::Value parameter_value(rapidjson::kObjectType);
          if (parameter.second.type == "string")
          {
            rapidjson::Value pvf_value(rapidjson::kStringType);
            pvf_value.SetString(parameter.second.value_string.c_str(), parameter.second.value_string.size());
            parameter_value.AddMember("pvf_type", "string", allocator);
            parameter_value.AddMember("pvf_value", pvf_value, allocator);
          }
          else if (parameter.second.type == "number")
          {
            rapidjson::Value pvf_value(rapidjson::kNumberType);
            pvf_value.SetDouble(parameter.second.value_number);
            parameter_value.AddMember("pvf_type", "number", allocator);
            parameter_value.AddMember("pvf_value", pvf_value, allocator);
          }
          object.AddMember(parameter_name, parameter_value, allocator); 
        }
        fromScratch.AddMember("input_parameters", object, allocator);

        /*
         * Convert the JSON datastructure to a JSON string
         */
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        fromScratch.Accept(writer);
        std::cout << strbuf.GetString() << std::endl;

        /*
         * Create a UMRF graph ROS message and publish it
         */ 
        temoto_action_engine::UmrfJsonGraph umrf_json_graph_msg;
        umrf_json_graph_msg.graph_name = query_text;
        umrf_json_graph_msg.name_match_required = 0;
        umrf_json_graph_msg.umrf_json_strings.push_back(strbuf.GetString());
        umrf_json_graph_pub_.publish(umrf_json_graph_msg);

      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    else
    {
      //TEMOTO_PRINT_OF("No new Google Actions.", getUmrfPtr()->getFullName());
    }

    ros::Duration(2).sleep();
  }
}

const rapidjson::Value& getRootJsonElement(const char* element_name, const rapidjson::Document& json_doc)
{
  if(!json_doc.HasMember(element_name))
  {
    throw CREATE_TEMOTO_ERROR_STACK("This JSON does not contain element '" + std::string(element_name) + "'");
  }
  return json_doc[element_name];
}

const rapidjson::Value& getJsonElement(const char* element_name, const rapidjson::Value& value_in)
{
  if(!value_in.HasMember(element_name))
  {
    throw CREATE_TEMOTO_ERROR_STACK("This field does not contain element '" + std::string(element_name) + "'");
  }
  return value_in[element_name];
}

std::string getStringFromValue(const rapidjson::Value& value)
{
  if (!value.IsString())
  {
    throw CREATE_TEMOTO_ERROR_STACK("This JSON value is not a string");
  }
  return value.GetString();
}

double getNumberFromValue(const rapidjson::Value& value)
{
  if (!value.IsNumber())
  {
    throw CREATE_TEMOTO_ERROR_STACK("This JSON value is not a number");
  }
  return value.GetDouble();
}

// Destructor
~TaGoogleAssistantParser()
{
  // ---> YOUR CONSTRUCTION ROUTINES HERE <--- //
  TEMOTO_PRINT_OF("Destructor", getUmrfPtr()->getName());
}

ros::NodeHandle nh_;
ros::Publisher umrf_json_graph_pub_;
std::string response_str;

}; // TaGoogleAssistantParser class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaGoogleAssistantParser, ActionBase);
