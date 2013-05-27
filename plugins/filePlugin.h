#ifndef FILEPLUGIN_H
#define FILEPLUGIN_H

#include "../plugin.h"

#include <fstream>
#include "../http.h"

using namespace std;

class filePlugin:public plugin
{
	private:
	public:
		int handleCommand(const string& /*nick*/, const string& /*channel*/, const vector<string>& /*words*/)
		{
			return 0;
		}

		int doTick()
		{
			return 0;
		}

		static void loadFile(const string& fileName, stringstream &result)
		{
			ifstream questionFile(fileName.c_str(), ifstream::in);
			if(questionFile.good())
			{
				string buffer;
				while(getline(questionFile, buffer))
				{
					result << buffer + "\n";
				}
				questionFile.close();
			}
			else
			{
				result.setstate(ios_base::failbit);
			}
		}

		static void loadHttp(const string& url, stringstream &result)
		{
			string hostname = stringUtils::urlHostname(url);
			string path = stringUtils::urlPath(url);
			string port = stringUtils::urlPort(url);
			cout << "Getting " << hostname << " " << path << " " << port << endl;
			http httpNet(hostname, port);
			string httpData = httpNet.get(path);
			result << httpData;
		}
};

#endif
