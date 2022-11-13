using Bt.DeviceApi.Particle;
using Bt.DeviceApi.Worker;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.

builder.Services.AddSingleton<Cloud>();
builder.Services.AddHttpClient<Cloud>(client =>
{
    client.BaseAddress = new Uri("https://api.particle.io/v1/");
    client.DefaultRequestHeaders.Add("Authorization", $" Bearer {builder.Configuration["Particle:AccessToken"]}");
});

JsonConvert.DefaultSettings = () => new JsonSerializerSettings
{
    ContractResolver = new CamelCasePropertyNamesContractResolver(),
    DateTimeZoneHandling = DateTimeZoneHandling.Local
};

builder.Services.AddControllers();
builder.Services.Configure<RouteOptions>(options => options.LowercaseUrls = true);

builder.Services.AddHostedService<ParticleListenerHostedService>();

// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

// Configure the HTTP request pipeline.
/*
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}
*/

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.Run();

