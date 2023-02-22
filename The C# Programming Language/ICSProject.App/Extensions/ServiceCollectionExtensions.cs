using System;
using Microsoft.Extensions.DependencyInjection;
using ICSProject.App.Factories;

namespace ICSProject.App.Extensions
{
    public static class ServiceCollectionExtensions
    {
        public static void AddFactory<TService, TImplementation>(this IServiceCollection services)
            where TService : class
            where TImplementation : class, TService
        {
            // generates new instances
            services.AddTransient<TService, TImplementation>();

            // interface for generating
            services.AddSingleton<Func<TService>>(x => x.GetRequiredService<TService>);

            services.AddSingleton<IFactory<TService>, Factory<TService>>();
        }
    }
}