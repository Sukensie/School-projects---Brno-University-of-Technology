using ICSProject.BL.Models;

namespace ICSProject.App.Messages
{
    public record AddedMessage<T> : Message<T> where T : IModel { }
}