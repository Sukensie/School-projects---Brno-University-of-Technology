using System;
using System.Runtime.Serialization;
using System.Windows;
using ICSProject.App.Extensions;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Options;

using ICSProject.App.ViewModels;
using ICSProject.App.Settings;
using ICSProject.App.Views;
using ICSProject.App.Messages;
using ICSProject.App.Services;
using ICSProject.App.Stores;
using ICSProject.DAL;
using ICSProject.BL;
using ICSProject.DAL.Factories;

namespace ICSProject.App
{
    public partial class App : Application
    {
        private readonly IHost _host;

        public App()
        {
            //Thread.CurrentThread.CurrentCulture = new CultureInfo("cs");
            //Thread.CurrentThread.CurrentUICulture = new CultureInfo("cs");

            _host = Host.CreateDefaultBuilder()
                .ConfigureAppConfiguration(ConfigureAppConfiguration)
                .ConfigureServices((context, services) => { ConfigureServices(context.Configuration, services); })
                .Build();
        }

        private static void ConfigureAppConfiguration(HostBuilderContext context, IConfigurationBuilder builder)
        {
            builder.AddJsonFile(@"AppSettings.json", false, false);
        }

        private static void ConfigureServices(IConfiguration configuration,
            IServiceCollection services)
        {
            services.AddBLServices();

            services.Configure<DALSettings>(configuration.GetSection("ICSProject:DAL"));

            services.AddSingleton<IDbContextFactory<ICSProjectDbContext>>(provider =>
            {
                var dalSettings = provider.GetRequiredService<IOptions<DALSettings>>().Value;
                return new SqlServerDbContextFactory(dalSettings.ConnectionString!, dalSettings.SkipMigrationAndSeedDemoData);
             });


            //Append ViewModels
            services.AddSingleton<MainWindow>();
            services.AddSingleton<MainViewModel>();


            services.AddSingleton<IMediator, Mediator>();
            services.AddSingleton<IMessageDialogService, MessageDialogService>();

            services.AddSingleton<LoginViewModel>();
            services.AddSingleton<UsersListViewModel>();
            services.AddSingleton<RideDetailViewModel>();
            services.AddSingleton<RideViewModel>();
            services.AddSingleton<RideSearchListViewModel>();
            services.AddSingleton<CreateUserViewModel>();
            services.AddSingleton<CreateRideViewModel>();
            services.AddSingleton<UpdateRideViewModel>();
            services.AddSingleton<GarageViewModel>();
            services.AddSingleton<CarsListViewModel>();
            services.AddSingleton<ProfileViewModel>();
            services.AddSingleton<MenuViewModel>();



            services.AddSingleton<NavigationStore>();



            //services.AddSingleton<IRecipeListViewModel, RecipeListViewModel>();
            


            //services.AddFactory<IIngredientDetailViewModel, IngredientDetailViewModel>();
            //services.AddFactory<IRecipeDetailViewModel, RecipeDetailViewModel>();
            //services.AddFactory<IIngredientAmountDetailViewModel, IngredientAmountDetailViewModel>();
        }

        protected override async void OnStartup(StartupEventArgs e)
        {
            await _host.StartAsync();

            var dbContextFactory = _host.Services.GetRequiredService<IDbContextFactory<ICSProjectDbContext>>();

            var dalSettings = _host.Services.GetRequiredService<IOptions<DALSettings>>().Value;

            await using (var dbx = await dbContextFactory.CreateDbContextAsync())
            {
                if (dalSettings.SkipMigrationAndSeedDemoData)
                {
                    await dbx.Database.EnsureCreatedAsync();
                }
                else
                {
                    await dbx.Database.MigrateAsync();
                }
            }

            var startupWindow = _host.Services.GetRequiredService<MainWindow>();
            startupWindow.Show();

            base.OnStartup(e);
        }

        protected override async void OnExit(ExitEventArgs e)
        {
            using (_host)
            {
                await _host.StopAsync(TimeSpan.FromSeconds(15));
            }

            base.OnExit(e);
        }
    }
}
