namespace ICSProject.DAL.Entities;

public record CarEntity(
    Guid Id,
    string Manufacturer,
    string Type,
    DateTime RegistrationTime,
    uint NumberOfSeats,
    string? ImageUrl,
    Guid OwnerId) : IEntity

{
    //Automapper requires parameter less constructor for collection synchronization for now
#nullable disable
    public CarEntity() : this(default, default, default, default, default, default, default) { }
#nullable enable

    public UserEntity? Owner { get; init; }
    public ICollection<RideEntity> Rides { get; init; } = new List<RideEntity>();
}