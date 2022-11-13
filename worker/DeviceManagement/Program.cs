
using Newtonsoft.Json;

HttpClient client = new HttpClient();
client.Timeout = TimeSpan.FromSeconds(5);
string url = $"https://api.particle.io/v1/devices/events/bt%2Fdev?access_token=eadbed9fe28055ec99e9f9e506a9f74c3fe127b6";

while (true)
{
    try
    {
        Console.WriteLine("Establishing connection");
        using (var streamReader = new StreamReader(await client.GetStreamAsync(url)))
        {
            string lastEventName = "";
            while (!streamReader.EndOfStream)
            {
                var line = await streamReader.ReadLineAsync();
                if(line == null){
                    continue;
                }
                if (line.StartsWith("event:"))
                {
                    lastEventName = line.Substring(6).Trim();
                }else if (line.StartsWith("data:") && !string.IsNullOrWhiteSpace(lastEventName))
                {
                    var jsonSerializerSettings = new JsonSerializerSettings() { DateTimeZoneHandling = DateTimeZoneHandling.Local };
                    var particleEventResponse = JsonConvert.DeserializeObject<ParticleEventResponse>(line.Substring(5), jsonSerializerSettings);
                    particleEventResponse.Name = lastEventName;
                    lastEventName = "";

                    Console.WriteLine($"=> {JsonConvert.SerializeObject(particleEventResponse)}");
                }



                
            }
        }
    }
    catch(Exception ex)
    {
        //Here you can check for 
        //specific types of errors before continuing
        //Since this is a simple example, i'm always going to retry
        Console.WriteLine($"Error: {ex.Message}");
        Console.WriteLine("Retrying in 5 seconds");
        await Task.Delay(TimeSpan.FromSeconds(5));
    }
}