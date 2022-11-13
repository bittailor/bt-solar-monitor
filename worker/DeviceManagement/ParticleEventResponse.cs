using Newtonsoft.Json;

public class ParticleEventResponse
{
    public string Name { get; set; }

    [JsonProperty("data")]
    public string Data { get; set; }
    [JsonProperty("ttl")]
    public int TTL { get; set; }
    [JsonProperty("published_at")]
    public DateTime PublishedAt { get; set; }
    [JsonProperty("coreid")]
    public string DeviceId { get; set; }
}