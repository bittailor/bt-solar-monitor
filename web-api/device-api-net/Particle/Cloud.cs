using System.Reactive.Linq;
using System.Web;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Bt.DeviceApi.Particle;

public class Cloud
{
    private readonly ILogger<Cloud> _logger;
    private readonly HttpClient _httpClient;
    

    public Cloud(ILogger<Cloud> logger, HttpClient httpClient)
    {
        _logger = logger;
        _httpClient = httpClient;
    }

    public IObservable<Event> Subscribe(string eventPrefix)
    {
        return Observable.Create(async (IObserver<Event> observer, CancellationToken cancellationToken) =>
        {
            var url = $"devices/events/{HttpUtility.UrlEncode(eventPrefix)}";
            while (!cancellationToken.IsCancellationRequested)
            {
                try
                {
                    _logger.LogInformation("Establishing server-sent events connection to {url}", url);
                    using var streamReader = new StreamReader(await _httpClient.GetStreamAsync(url, cancellationToken));
                    cancellationToken.Register(() => { streamReader.Close();});
                    await PollForEvents(streamReader, cancellationToken, observer);
                }
                catch(Exception ex)
                {
                    if (!cancellationToken.IsCancellationRequested)
                    {
                        _logger.LogError(ex,"server-sent events connection connection failed {message}",ex.Message);
                        _logger.LogInformation("Retrying in 5 seconds");
                        await Task.Delay(TimeSpan.FromSeconds(5), cancellationToken);
                    }
                }    
            }
        });
    }

    private async Task PollForEvents(StreamReader streamReader, CancellationToken cancellationToken, IObserver<Event> observer)
    {
        var lastEventName = "";
        while (!streamReader.EndOfStream && !cancellationToken.IsCancellationRequested)
        {
            var line = await streamReader.ReadLineAsync();
            if (line == null) { continue; }

            if (line.StartsWith("event:"))
            {
                lastEventName = line.Substring(6).Trim();
            }
            else if (line.StartsWith("data:") && !string.IsNullOrWhiteSpace(lastEventName))
            {
                var dto = JsonConvert.DeserializeObject<EventResponseDto>(line.Substring(5));
                if (dto == null) { continue; }
                
                var @event = new Event(lastEventName, dto.Data, dto.TTL, dto.PublishedAt, dto.DeviceId);
                lastEventName = "";
                _logger.LogInformation("Event {event}", @event);
                observer.OnNext(@event);
            }
        }
    }

    public async Task<(bool success, int @return)> CallFunction(string deviceId, string name, string argument)
    {
        var request = new HttpRequestMessage()
        {
            RequestUri = new Uri($"devices/{deviceId}/{name}", UriKind.RelativeOrAbsolute),
            Method = HttpMethod.Post,
            Content = new FormUrlEncodedContent(new Dictionary<string, string>()
            {
                { "arg", argument }
            })
        };
        _logger.LogDebug("call function {deviceId} {name} {argument} -> {request}",deviceId, name, argument,request);
        var response =  await _httpClient.SendAsync(request);
        if (!response.IsSuccessStatusCode)
        {
            _logger.LogWarning("Publish failed {request} => {response}",request,response);
            return (false,0);
        }
        dynamic content = JObject.Parse(await response.Content.ReadAsStringAsync());
        return (true,content.return_value);
    }

    public async Task<bool> Publish(string name, string data)
    {
        var parameters = new Dictionary<string,string>()
        {
            { "name", name },
            { "data", data },
            { "private", "true" }
        };
        
        var response =  await _httpClient.PostAsync("devices/events", new FormUrlEncodedContent(parameters));

        if (!response.IsSuccessStatusCode)
        {
            _logger.LogWarning("Publish failed {response}",response);
            return false;
        }
        dynamic content = JObject.Parse(await response.Content.ReadAsStringAsync());
        return content.ok;
    }
}