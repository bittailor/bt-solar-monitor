using Bt.DeviceApi.Particle;

namespace Bt.DeviceApi.Worker;

public class ParticleListenerHostedService : IHostedService, IDisposable
{
    private readonly ILogger<ParticleListenerHostedService> _logger;
    private readonly Cloud _cloud;
    private readonly Dictionary<string, Action<Event>> _stateHandlers;
    private IDisposable? _deviceStateSubscription;
    

    public ParticleListenerHostedService(ILogger<ParticleListenerHostedService> logger, Cloud cloud)
    {
        _logger = logger;
        _cloud = cloud;
        _stateHandlers = new Dictionary<string, Action<Event>>()
        {
            {"online", OnOnline},
            {"offline", OnOffline}
        };
    }
    
    public Task StartAsync(CancellationToken cancellationToken)
    {
        _logger.LogInformation("Starting Service ...");
        _deviceStateSubscription = _cloud.Subscribe("bt/device/state").Subscribe(HandleEvent);
        _logger.LogInformation("... done");
        return Task.CompletedTask;
    }
    public Task StopAsync(CancellationToken cancellationToken)
    {
        _logger.LogInformation("Stopping Service ...");
        _deviceStateSubscription?.Dispose();
        _deviceStateSubscription = null;
        _logger.LogInformation("... done");
        return Task.CompletedTask;
    }

    public void Dispose()
    {
        _deviceStateSubscription?.Dispose();
    }
    
    private void HandleEvent(Event @event)
    {
        var state = @event.Name.Split("/").Last();
        if (_stateHandlers.TryGetValue(state, out var handler))
        {
            handler(@event);
            return;
        }
        _logger.LogWarning("No handler for {state}", state);
    }
    
    private void OnOnline(Event @event)
    {
        /*
        var cmd = new StayOnlineCmdDto()
        {
            Second = 0
        };
        _cloud.Publish($"bt/{@event.DeviceId}/cmd", JsonConvert.SerializeObject(cmd)).ContinueWith((task) =>
        {
            _logger.LogInformation("published response => {event}", task.Result);     
        });
        */
        _logger.LogInformation("OnOnline => Cloud-updateOnlinePeriod");
        _cloud.CallFunction(@event.DeviceId, "Cloud-updateOnlinePeriod", "0")
        .ContinueWith((task) =>
        {
            if (task.IsFaulted)
            {
                _logger.LogError(task.Exception ,"Call function Cloud-updateOnlinePeriod failed :{message}", task.Exception?.Message);    
            }
            _logger.LogInformation("published response => {response}", task.Result);     
        });
    }
    
    private void OnOffline(Event @event)
    {
    }

}