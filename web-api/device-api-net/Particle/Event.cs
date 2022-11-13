namespace Bt.DeviceApi.Particle;

public record Event(string Name, string Data, int Ttl, DateTime PublishedAt, string DeviceId);