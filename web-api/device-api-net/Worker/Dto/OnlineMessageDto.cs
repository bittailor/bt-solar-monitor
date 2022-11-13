using Newtonsoft.Json;

namespace Bt.DeviceApi.Worker.Dto;

public class OnlineMessageDto
{
    [JsonProperty("ttc")]
    public int TimeToConnect { get; set; }
    [JsonProperty("signal")] 
    public SignalDto Signal { get; set; } = new();
}